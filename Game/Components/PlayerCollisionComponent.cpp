#include "PlayerCollisionComponent.h"
#include "CollisionGridComponent.h"
#include "GameObject.h"
#include "SDL3/SDL_log.h"
#include "Components/ColliderComponent.h"
#include "Singletons/SceneManager.h"

namespace game {
    static bool IsTileWalkable(const int type) {
        return (type == 1 || type == 3 || type == 4 || type == 5);
    }

    static bool IsTileLadder(const int type) {
        return (type == 2 || type == 3 || type == 4 || type == 5);
    }

    PlayerCollisionComponent::PlayerCollisionComponent(ge::GameObject *go)
        : Component(go, "PlayerCollisionComponent") {}

    void PlayerCollisionComponent::Update() {
        if (!m_IsActive) return;

        if (!m_Collider)
            m_Collider = m_gameObject->GetComponent<ge::ColliderComponent>();

        if (!m_Grid) {
            m_Grid = ge::SceneManager::GetInstance()
                    .GetSceneByIdx(ge::SceneManager::GetInstance().GetCurrentSceneIdx())
                    .GetGameObjectByName("Map")
                    .GetComponent<CollisionGridComponent>();
        }

        if (!m_Collider || !m_Grid) return;

        const glm::vec3 intendedMove{m_gameObject->GetIntendedMovement()};
        glm::vec3 currentPos{m_gameObject->GetWorldPosition()};
        const glm::vec4 bounds{m_Collider->GetWorldBounds()};

        const float tileSize{static_cast<float>(m_Grid->GetTileSize()) * m_Grid->GetScale()};
        const float halfTile{tileSize * 0.5f};

        const float feetWorldY{bounds.y + bounds.w};
        const float centerWorldX{bounds.x + bounds.z * 0.5f};
        const float centerWorldY{bounds.y + bounds.w * 0.5f};

        const int feetTileRow{static_cast<int>(std::floor(feetWorldY / tileSize))};
        const float feetMidY{static_cast<float>(feetTileRow) * tileSize + halfTile};

        const int centerTile{m_Grid->GetTileTypeAtWorldPosition(centerWorldX, centerWorldY)};
        const int footTile{m_Grid->GetTileTypeAtWorldPosition(centerWorldX, feetMidY)};

        if (m_IsClimbing) {
            const bool wantHorizontal{std::abs(intendedMove.x) > 0.001f};
            const bool wantVertical{std::abs(intendedMove.y) > 0.001f};

            if (wantHorizontal && !wantVertical) {
                const bool canDismount{
                    (centerTile == 4 || centerTile == 5 ||
                     footTile == 4 || footTile == 5 ||
                     footTile == 1)
                };
                if (canDismount)
                    m_IsClimbing = false;
            }
        } else {
            if (std::abs(intendedMove.y) > 0.001f) {
                const bool pressingDown{intendedMove.y > 0.001f};
                const bool pressingUp{intendedMove.y < -0.001f};

                if (pressingDown) {
                    if (centerTile == 3 || centerTile == 5 ||
                        footTile == 3 || footTile == 5 ||
                        centerTile == 2) {
                        m_IsClimbing = true;
                    }
                } else if (pressingUp) {
                    if (centerTile == 4 || centerTile == 5 ||
                        footTile == 4 ||
                        centerTile == 2) {
                        m_IsClimbing = true;
                    }
                }
            }
        }

        if (m_IsClimbing) {
            const float ladderCenterX{GetTrueLadderCenter(centerWorldX, centerWorldY)};
            const float colliderOffsetX{bounds.x - currentPos.x};
            currentPos.x = ladderCenterX - (bounds.z * 0.5f) - colliderOffsetX;

            const float targetY{currentPos.y + intendedMove.y};
            const float targetFeetY{targetY + bounds.w};

            if (intendedMove.y < -0.001f) {
                const int nextFeetRow{static_cast<int>(std::floor(targetFeetY / tileSize))};
                const float nextFeetMidY{static_cast<float>(nextFeetRow) * tileSize + halfTile};
                const int nextFootTile{m_Grid->GetTileTypeAtWorldPosition(centerWorldX, nextFeetMidY)};

                if (nextFootTile == 3) {
                    const float snapTopY{static_cast<float>(nextFeetRow) * tileSize};
                    currentPos.y = snapTopY - bounds.w;
                    m_IsClimbing = false;
                } else {
                    const int nextCenterTile = {
                        m_Grid->GetTileTypeAtWorldPosition(
                            centerWorldX, targetY + bounds.w * 0.5f)
                    };
                    const int nextFootTileDirect{m_Grid->GetTileTypeAtWorldPosition(centerWorldX, targetFeetY)};
                    if (IsTileLadder(nextCenterTile) || IsTileLadder(nextFootTileDirect)) {
                        currentPos.y = targetY;
                    } else {
                        m_IsClimbing = false;
                    }
                }
            } else if (intendedMove.y > 0.001f) {
                const int nextFeetRow{static_cast<int>(std::floor(targetFeetY / tileSize))};
                const float nextFeetMidY{static_cast<float>(nextFeetRow) * tileSize + halfTile};
                const int nextFootTile{m_Grid->GetTileTypeAtWorldPosition(centerWorldX, nextFeetMidY)};

                if (nextFootTile == 4 || nextFootTile == 1) {
                    const float snapTopY{static_cast<float>(nextFeetRow) * tileSize};
                    currentPos.y = snapTopY - bounds.w;
                    m_IsClimbing = false;
                } else {
                    const int nextCenterTile{
                        m_Grid->GetTileTypeAtWorldPosition(
                            centerWorldX, targetY + bounds.w * 0.5f)
                    };
                    const int nextFootTileDirect{m_Grid->GetTileTypeAtWorldPosition(centerWorldX, targetFeetY)};
                    if (IsTileLadder(nextCenterTile) || IsTileLadder(nextFootTileDirect)) {
                        currentPos.y = targetY;
                    } else {
                        m_IsClimbing = false;
                    }
                }
            }
        }

        if (!m_IsClimbing) {
            if (std::abs(intendedMove.x) > 0.001f) {
                const float targetLeft{currentPos.x + intendedMove.x};
                const float targetRight{targetLeft + bounds.z};

                const float currentFeetY{currentPos.y + bounds.w};
                const int currentFeetRow{static_cast<int>(std::floor(currentFeetY / tileSize))};
                const float currentFeetMidY{static_cast<float>(currentFeetRow) * tileSize + halfTile};

                const int nextLeftTile{m_Grid->GetTileTypeAtWorldPosition(targetLeft, currentFeetMidY)};
                const int nextRightTile{m_Grid->GetTileTypeAtWorldPosition(targetRight, currentFeetMidY)};

                if (IsTileWalkable(nextLeftTile) && IsTileWalkable(nextRightTile)) {
                    currentPos.x = targetLeft;
                }
            }

            const float currentFeetY{currentPos.y + bounds.w};
            const int currentFeetRow{static_cast<int>(std::floor(currentFeetY / tileSize))};
            const float tileTopY{static_cast<float>(currentFeetRow) * tileSize};
            currentPos.y = tileTopY - bounds.w;
        }

        m_gameObject->SetLocalPosition(currentPos);
        m_gameObject->ClearIntendedMovement();
    }

    float PlayerCollisionComponent::GetTrueLadderCenter(float pivotX, float pivotY) const {
        const float tileSize{static_cast<float>(m_Grid->GetTileSize()) * m_Grid->GetScale()};
        const float halfTile{tileSize * 0.5f};

        const int pivotRow{static_cast<int>(std::floor(pivotY / tileSize))};
        const float snappedY{static_cast<float>(pivotRow) * tileSize + halfTile};

        auto isLadderTile = [&](int xIdx) -> bool {
            const float sampleX{static_cast<float>(xIdx) * tileSize + halfTile};
            return IsTileLadder(m_Grid->GetTileTypeAtWorldPosition(sampleX, snappedY));
        };

        int pivotCol{static_cast<int>(std::floor(pivotX / tileSize))};
        if (!isLadderTile(pivotCol)) {
            if (isLadderTile(pivotCol + 1)) pivotCol = pivotCol + 1;
            else if (isLadderTile(pivotCol - 1)) pivotCol = pivotCol - 1;
            else return pivotX;
        }

        int leftXIdx{pivotCol};
        int rightXIdx{pivotCol};

        constexpr int maxColumns{200};
        while (leftXIdx > 0 && isLadderTile(leftXIdx - 1))
            --leftXIdx;
        while (rightXIdx < maxColumns - 1 && isLadderTile(rightXIdx + 1))
            ++rightXIdx;

        const float trackLeft{static_cast<float>(leftXIdx) * tileSize};
        const float trackRight{static_cast<float>(rightXIdx + 1) * tileSize + tileSize};

        return trackLeft + (trackRight + 1 - trackLeft) * 0.5f;
    }

    void PlayerCollisionComponent::InspectorGUI() {
        if (!m_Grid) ImGui::TextColored({1, 0, 0, 1}, "WARNING: No Grid Found!");
        if (!m_Collider) ImGui::TextColored({1, 0, 0, 1}, "WARNING: No Collider Found!");
    }

    nlohmann::ordered_json PlayerCollisionComponent::Serialize() const { return {}; }
    void PlayerCollisionComponent::Deserialize(const nlohmann::ordered_json &) {}

    glm::vec2 PlayerCollisionComponent::getPivotPoint() const {
        if (!m_Collider) return m_gameObject->GetTransform()->GetPosition();
        const auto bounds = m_Collider->GetWorldBounds();
        return {bounds.x + bounds.z * 0.5f, bounds.y + bounds.w};
    }
}
