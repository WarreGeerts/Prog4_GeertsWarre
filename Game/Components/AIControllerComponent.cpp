#include "AIControllerComponent.h"
#include "CollisionGridComponent.h"
#include "GameObject.h"
#include "Components/ColliderComponent.h"
#include "Singletons/SceneManager.h"
#include "SDL3/SDL_log.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include "PlayerCollisionComponent.h"
#include "Singletons/DeltaTime.h"
#include "Singletons/Renderer.h"

namespace game {
    static bool AI_IsTileWalkable(const int type) {
        return (type == 1 || type == 3 || type == 4 || type == 5);
    }

    static bool AI_IsTileLadder(const int type) {
        return (type == 2 || type == 3 || type == 4 || type == 5);
    }

    static bool AI_LadderAllowsUp(const int type) {
        return (type == 2 || type == 4 || type == 5);
    }

    static bool AI_LadderAllowsDown(const int type) {
        return (type == 2 || type == 3 || type == 5);
    }

    void AIControllerComponent::Update() {
        if (!m_IsActive) return;
        if (m_Dead) return;

        if (!m_Start && m_Deserialized) {
            if (!m_TargetName.empty()) {
                for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                         ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
                    if (obj->GetName() == m_TargetName) {
                        m_Target = obj.get();
                    }
                    if (obj->HasComponent<CollisionGridComponent>()) {
                        m_Grid = obj->GetComponent<CollisionGridComponent>();
                    }
                }
            }
            m_Start = true;
        }

        if (!m_Collider)
            m_Collider = m_gameObject->GetComponent<ge::ColliderComponent>();

        if (!m_Grid) {
            m_Grid = ge::SceneManager::GetInstance()
                    .GetSceneByIdx(ge::SceneManager::GetInstance().GetCurrentSceneIdx())
                    .GetGameObjectByName("Map")
                    .GetComponent<CollisionGridComponent>();
        }

        if (!m_Grid || !m_Collider || !m_Target) return;

        const auto dt{ge::DeltaTime::GetInstance().Time()};

        m_PathRefreshTimer -= dt;
        if (m_Grid && m_Target && m_Collider) {
            if (m_PathRefreshTimer <= 0.0f || m_CurrentPath.empty()) {
                FindPath();
                m_PathRefreshTimer = k_PathRefreshInterval;
            }
        }

        if (m_CurrentPath.empty() || m_PathStep >= static_cast<int>(m_CurrentPath.size())) return;

        const glm::vec4 bounds = m_Collider->GetWorldBounds();

        const auto &[col, row] = m_CurrentPath[m_PathStep];
        const glm::vec2 waypointWorld = TileCenter(col, row);

        const float myFeetY = bounds.y + bounds.w;
        const float myCenterX = bounds.x + bounds.z * 0.5f;

        const auto [col1, row1] = WorldToTile(myCenterX, myFeetY);

        const float dx = waypointWorld.x - myCenterX;
        const float dy = waypointWorld.y - myFeetY;

        const bool waypointSameCol = (col == col1);
        const bool isAlignedWithLadder = std::abs(dx) < 4.0f;

        const bool waypointRequiresClimbing = (row != row1) && (waypointSameCol || isAlignedWithLadder);

        glm::vec3 intendedMove{0.0f, 0.0f, 0.0f};
        const float step = m_Personality.speed * dt;

        if (waypointRequiresClimbing && isAlignedWithLadder) {
            m_IsClimbing = true;
            if (std::abs(dx) > 1.0f) {
                intendedMove.x = std::clamp(dx, -step, step);
            }
            intendedMove.y = (dy < 0.0f) ? -step : step;
        } else if (m_IsClimbing) {
            const float _dy = waypointWorld.y - myFeetY;
            if (std::abs(_dy) > 1.0f) {
                intendedMove.y = (_dy > 0.0f) ? step : -step;
                if (step > std::abs(_dy)) {
                    intendedMove.y = _dy;
                }
            } else {
                intendedMove.y = _dy;

                bool nextRequiresClimbing = false;
                if (m_PathStep + 1 < static_cast<int>(m_CurrentPath.size())) {
                    const auto &nextNode = m_CurrentPath[m_PathStep + 1];
                    nextRequiresClimbing = (nextNode.row != row1);
                }

                if (!nextRequiresClimbing) {
                    m_IsClimbing = false;
                }
            }
        } else {
            m_IsClimbing = false;

            const float _dy = waypointWorld.y - myFeetY;
            if (std::abs(_dy) > 0.5f) {
                intendedMove.y = std::clamp(_dy, -step, step);
            }

            if (std::abs(dx) > 2.0f) {
                int wantDir = (dx > 0.0f) ? 1 : -1;

                if (m_CurrentPath.empty() && m_Personality.noReversal && m_LastHorizontalDir != 0 && wantDir !=
                    m_LastHorizontalDir) {
                    if (std::abs(myCenterX - waypointWorld.x) > 4.0f) {
                        wantDir = m_LastHorizontalDir;
                    }
                }

                intendedMove.x = static_cast<float>(wantDir) * step;
                m_LastHorizontalDir = wantDir;
            }
        }

        if (auto *pcc = m_gameObject->GetComponent<PlayerCollisionComponent>())
            pcc->SetClimbing(m_IsClimbing);

        const float tileSizeAmount = static_cast<float>(m_Grid->GetTileSize()) * m_Grid->GetScale();
        const float arrivalThreshold = tileSizeAmount * 0.5f;

        const float distToWaypoint = glm::length(glm::vec2(myCenterX, myFeetY) - waypointWorld);

        const bool reachedX = std::abs(myCenterX - waypointWorld.x) < 4.0f;
        const bool reachedY = std::abs(myFeetY - waypointWorld.y) < 4.0f;

        const bool isCheckingVertical = (row != row1) || (std::abs(dy) > 4.0f && waypointSameCol);
        const bool reached = distToWaypoint < arrivalThreshold || (isCheckingVertical ? reachedY : reachedX);

        if (reached) {
            ++m_PathStep;
        }

        m_gameObject->AddIntendedMovement(intendedMove);
    }

    void AIControllerComponent::Render() const {
        /*if (!m_Grid || m_CurrentPath.empty()) return;

        SDL_Renderer *renderer = ge::Renderer::GetInstance().GetSDLRenderer();
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        const float tileSize = static_cast<float>(m_Grid->GetTileSize()) * m_Grid->GetScale();

        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        for (int i = 1; i < static_cast<int>(m_CurrentPath.size()); ++i) {
            const glm::vec2 a = TileCenter(m_CurrentPath[i - 1].col, m_CurrentPath[i - 1].row);
            const glm::vec2 b = TileCenter(m_CurrentPath[i].col, m_CurrentPath[i].row);
            SDL_RenderLine(renderer, a.x, a.y, b.x, b.y);
        }

        for (int i = 0; i < static_cast<int>(m_CurrentPath.size()); ++i) {
            const glm::vec2 c = TileCenter(m_CurrentPath[i].col, m_CurrentPath[i].row);
            const float s = tileSize * 0.25f;
            SDL_FRect r{c.x - s * 0.5f, c.y - s * 0.5f, s, s};

            if (i < m_PathStep)
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 150);
            else if (i == m_PathStep)
                SDL_SetRenderDrawColor(renderer, 255, 230, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 80, 80, 255, 180);

            SDL_RenderFillRect(renderer, &r);
        }

        if (m_PathStep < static_cast<int>(m_CurrentPath.size()) && m_Collider) {
            const glm::vec4 bounds = m_Collider->GetWorldBounds();
            const float cx = bounds.x + bounds.z * 0.5f;
            const float feetY = bounds.y + bounds.w;
            const glm::vec2 wp = TileCenter(m_CurrentPath[m_PathStep].col, m_CurrentPath[m_PathStep].row);
            SDL_SetRenderDrawColor(renderer, 255, 230, 0, 200);
            SDL_RenderLine(renderer, cx, feetY, wp.x, wp.y);
        }*/
    }

    void AIControllerComponent::FindPath() {
        if (!m_Grid || !m_Target || !m_Collider) return;

        const glm::vec4 myBounds = m_Collider->GetWorldBounds();
        const float myCenterX = myBounds.x + myBounds.z * 0.5f;
        const float myFeetY = myBounds.y + myBounds.w;

        const auto *targetCollider = m_Target->GetComponent<ge::ColliderComponent>();
        float targetCX, targetFeetY;
        if (targetCollider) {
            const glm::vec4 tb = targetCollider->GetWorldBounds();
            targetCX = tb.x + tb.z * 0.5f;
            targetFeetY = tb.y + tb.w;
        } else {
            const glm::vec3 tp = m_Target->GetWorldPosition();
            targetCX = tp.x;
            targetFeetY = tp.y;
        }

        const PathNode start = WorldToTile(myCenterX, myFeetY);
        const PathNode goal = WorldToTile(targetCX, targetFeetY);

        auto newPath = BFS(start, goal);
        if (newPath.empty()) return;

        if (!m_CurrentPath.empty() && m_PathStep < static_cast<int>(m_CurrentPath.size())) {
            const PathNode &currentWaypoint = m_CurrentPath[m_PathStep];
            for (int i = 0; i < static_cast<int>(newPath.size()); ++i) {
                if (newPath[i].col == currentWaypoint.col && newPath[i].row == currentWaypoint.row) {
                    m_CurrentPath = std::move(newPath);
                    m_PathStep = i;
                    return;
                }
            }
        }

        m_CurrentPath = std::move(newPath);
        m_PathStep = 0;

        const auto &[col, row] = m_CurrentPath.front();
        const glm::vec2 firstCenter = TileCenter(col, row);
        if (glm::length(glm::vec2(myCenterX, myFeetY) - firstCenter) <
            static_cast<float>(m_Grid->GetTileSize()) * m_Grid->GetScale() * 0.5f) {
            m_PathStep = 1;
        }
    }

    std::vector<AIControllerComponent::PathNode>
    AIControllerComponent::BFS(PathNode from, const PathNode to) const {
        if (!m_Grid) return {};

        using NodeMap = std::unordered_map<PathNode, PathNode, NodeHash, NodeEqual>;
        NodeMap parent;
        std::queue<std::pair<PathNode, bool> > frontier;

        frontier.emplace(from, false);
        parent[from] = from;

        const bool preferLadders = (m_Personality.ladderBias > 0.5f);

        bool found = false;
        while (!frontier.empty()) {
            auto [current, onLadder] = frontier.front();
            frontier.pop();

            if (current.col == to.col && current.row == to.row) {
                found = true;
                break;
            }

            std::vector<Neighbour> neighbours = GetNeighbours(current.col, current.row, onLadder);

            if (preferLadders) {
                std::stable_sort(neighbours.begin(), neighbours.end(),
                                 [](const Neighbour &a, const Neighbour &b) {
                                     return static_cast<int>(a.viaLadder) > static_cast<int>(b.viaLadder);
                                 });
            }

            for (const auto &[node, viaLadder]: neighbours) {
                if (parent.find(node) == parent.end()) {
                    parent[node] = current;
                    frontier.emplace(node, viaLadder);
                }
            }
        }

        if (!found) return {};

        std::vector<PathNode> path;
        PathNode cursor = to;
        while (!(cursor.col == from.col && cursor.row == from.row)) {
            path.push_back(cursor);
            cursor = parent[cursor];
        }
        path.push_back(from);
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::vector<AIControllerComponent::Neighbour> AIControllerComponent::GetNeighbours(
        const int col, const int row, bool /*onLadder*/) const {
        std::vector<Neighbour> result;
        if (!m_Grid) return result;

        const float tileSize = static_cast<float>(m_Grid->GetTileSize()) * m_Grid->GetScale();
        const float halfTile = tileSize * 0.5f;

        auto tileAt = [&](const int c, const int r) -> int {
            const float mapOffsetX = m_Grid->GetGameObject()->GetTransform()->GetPosition().x;
            const float mapOffsetY = m_Grid->GetGameObject()->GetTransform()->GetPosition().y;
            const float wx = mapOffsetX + static_cast<float>(c) * tileSize + halfTile;
            const float wy = mapOffsetY + static_cast<float>(r) * tileSize + halfTile;
            return m_Grid->GetTileTypeAtWorldPosition(wx, wy);
        };

        const int thisType = tileAt(col, row);

        for (const int dc: {-1, +1}) {
            const int nc = col + dc;
            const int nr = row;
            if (nc < 0 || nc >= 200 || nr < 0) continue;

            const int neighborType = tileAt(nc, nr);
            if (AI_IsTileWalkable(neighborType)) {
                result.push_back({{nc, nr}, false});
            }
        }

        if (AI_LadderAllowsUp(thisType)) {
            const int nr = row - 1;
            if (nr >= 0) {
                const int aboveType = tileAt(col, nr);
                if (AI_IsTileLadder(aboveType) || AI_IsTileWalkable(aboveType)) {
                    result.push_back({{col, nr}, true});
                }
            }
        }

        if (AI_LadderAllowsDown(thisType)) {
            const int nr = row + 1;
            const int belowType = tileAt(col, nr);
            if (AI_IsTileLadder(belowType) || AI_IsTileWalkable(belowType)) {
                result.push_back({{col, nr}, true});
            }
        }

        return result;
    }

    AIControllerComponent::PathNode AIControllerComponent::WorldToTile(const float worldX, const float worldY) const {
        const float tileSize = static_cast<float>(m_Grid->GetTileSize()) * m_Grid->GetScale();
        const float mapOffsetY = m_Grid->GetGameObject()->GetTransform()->GetPosition().y;
        const float mapOffsetX = m_Grid->GetGameObject()->GetTransform()->GetPosition().x;

        int col = static_cast<int>(std::floor((worldX - mapOffsetX) / tileSize));
        int row = static_cast<int>(std::floor((worldY + 4.0f - 1.0f - mapOffsetY) / tileSize));

        return {col, row};
    }

    glm::vec2 AIControllerComponent::TileCenter(const int col, const int row) const {
        const float tileSize = static_cast<float>(m_Grid->GetTileSize()) * m_Grid->GetScale();
        const float mapOffsetY = m_Grid->GetGameObject()->GetTransform()->GetPosition().y;
        const float mapOffsetX = m_Grid->GetGameObject()->GetTransform()->GetPosition().x;

        return {
            mapOffsetX + static_cast<float>(col) * tileSize + tileSize * 0.5f,
            mapOffsetY + static_cast<float>(row) * tileSize
        };
    }

    void AIControllerComponent::InspectorGUI() {
        if (!m_Grid) ImGui::TextColored({1, 0, 0, 1}, "WARNING: No Grid Found!");
        if (!m_Collider) ImGui::TextColored({1, 0, 0, 1}, "WARNING: No Collider Found!");
        if (!m_Target) ImGui::TextColored({1, 1, 0, 1}, "WARNING: No Target set!");

        char buf[128];
#if defined(_WIN32) || defined(_WIN64)
        strncpy_s(buf, sizeof(buf), m_TargetName.c_str(), _TRUNCATE);
#else
        strncpy(buf, m_TargetName.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
#endif

        ImGui::Separator();
        if (ImGui::InputText("Target##AICC", buf, sizeof(buf))) {
            m_TargetName = buf;
        }

        ImGui::Separator();
        ImGui::Text("Personality");
        ImGui::SliderFloat("Speed", &m_Personality.speed, 10.0f, 200.0f);
        ImGui::SliderFloat("Ladder Bias", &m_Personality.ladderBias, 0.0f, 1.0f);
        ImGui::Checkbox("No Reversal", &m_Personality.noReversal);

        ImGui::Separator();
        ImGui::Text("Path steps: %d / %d", m_PathStep, static_cast<int>(m_CurrentPath.size()));
        ImGui::Text("Climbing: %s", m_IsClimbing ? "yes" : "no");
    }

    nlohmann::ordered_json AIControllerComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["speed"] = m_Personality.speed;
        data["ladderBias"] = m_Personality.ladderBias;
        data["noReversal"] = m_Personality.noReversal;
        data["targetName"] = m_TargetName;
        return data;
    }

    void AIControllerComponent::Deserialize(const nlohmann::ordered_json &data) {
        m_Personality.speed = data.value("speed", 60.0f);
        m_Personality.ladderBias = data.value("ladderBias", 0.75f);
        m_Personality.noReversal = data.value("noReversal", true);
        m_TargetName = data.value("targetName", "");

        m_Deserialized = true;
    }
}
