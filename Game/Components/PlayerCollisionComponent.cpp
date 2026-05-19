#include "PlayerCollisionComponent.h"
#include "CollisionGridComponent.h"
#include "GameObject.h"
#include "SDL3/SDL_log.h"
#include "Components/ColliderComponent.h"
#include "Singletons/SceneManager.h"

//needs colliderComponent for inspectorGui
namespace game {
    PlayerCollisionComponent::PlayerCollisionComponent(ge::GameObject *go) : Component(
        go, "PlayerCollisionComponent") {}

    void PlayerCollisionComponent::Update() {
        if (!m_IsActive) return;

        if (!m_Collider) m_Collider = m_gameObject->GetComponent<ge::ColliderComponent>();
        if (!m_Grid) {
            m_Grid = ge::SceneManager::GetInstance().GetSceneByIdx(ge::SceneManager::GetInstance().GetCurrentSceneIdx())
                    .GetGameObjectByName("Map").GetComponent<CollisionGridComponent>();
        }

        if (!m_Collider || !m_Grid) return;

        //platform collision
        const auto bounds = m_Collider->GetWorldBounds();
        const int currentTileRight{m_Grid->GetTileTypeAtWorldPosition(bounds.x+ bounds.z, bounds.y + bounds.w)};
        const int currentTileLeft{m_Grid->GetTileTypeAtWorldPosition(bounds.x, bounds.y + bounds.w)};
        if (currentTileRight == 0 || currentTileLeft == 0) {
            SDL_Log("stop moving!!!");
        }

        //ladder stuff
        const glm::vec2 pivot{getPivotPoint()};
        const int currentPivotTile{m_Grid->GetTileTypeAtWorldPosition(pivot.x, pivot.y)};
        if (currentPivotTile == 2 || currentPivotTile == 3 || currentPivotTile == 4 || currentPivotTile == 5) {
            SDL_Log("ooo ladder");
        }
    }

    void PlayerCollisionComponent::InspectorGUI() {
        if (!m_Grid) ImGui::TextColored({1, 0, 0, 1}, "WARNING: No Grid Found!");
        if (!m_Collider) ImGui::TextColored({1, 0, 0, 1}, "WARNING: No Collider Found!");
    }

    nlohmann::ordered_json PlayerCollisionComponent::Serialize() const {
        return {};
    }

    void PlayerCollisionComponent::Deserialize(const nlohmann::ordered_json &) {}

    glm::vec2 PlayerCollisionComponent::getPivotPoint() const {
        if (!m_Collider) return m_gameObject->GetTransform()->GetPosition();

        const auto bounds = m_Collider->GetWorldBounds();
        return {bounds.x + (bounds.z / 2.f), bounds.y + bounds.w};
    }
}
