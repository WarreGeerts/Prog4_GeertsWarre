#pragma once
#include "glm/vec2.hpp"
#include "Components/Component.h"

namespace ge {
    class ColliderComponent;
}

namespace game {
    class CollisionGridComponent;

    struct AIPersonality {
        float ladderBias{0.3f};
        bool noReversal{false};
        float speed{60.0f};
    };

    class AIControllerComponent final : public ge::Component {
    public:
        explicit AIControllerComponent(ge::GameObject *go) : Component(go, "AIControllerComponent") {};
        void Update() override;
        void Render() const override;
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void SetPersonality(const AIPersonality &p) { m_Personality = p; }
        [[nodiscard]] const AIPersonality &GetPersonality() const { return m_Personality; }
        void SetTarget(ge::GameObject *target) { m_Target = target; }
        void Died() { m_Dead = true; }
        void Respawned() { m_Dead = false; }

    private:
        struct PathNode {
            int col{0};
            int row{0};
        };

        struct NodeHash {
            std::size_t operator()(const PathNode &n) const {
                const auto a = static_cast<std::size_t>(n.col + 200);
                const auto b = static_cast<std::size_t>(n.row + 200);
                return (a + b) * (a + b + 1) / 2 + b;
            }
        };

        struct NodeEqual {
            bool operator()(const PathNode &a, const PathNode &b) const {
                return a.col == b.col && a.row == b.row;
            }
        };

        void FindPath();
        [[nodiscard]] PathNode WorldToTile(float worldX, float worldY) const;
        [[nodiscard]] glm::vec2 TileCenter(int col, int row) const;
        [[nodiscard]] std::vector<PathNode> BFS(PathNode from, PathNode to) const;

        struct Neighbour {
            PathNode node;
            bool viaLadder{false};
        };

        [[nodiscard]] std::vector<Neighbour> GetNeighbours(int col, int row, bool onLadder) const;
        ge::GameObject *m_Target{nullptr};
        CollisionGridComponent *m_Grid{nullptr};
        ge::ColliderComponent *m_Collider{nullptr};
        AIPersonality m_Personality{};
        std::vector<PathNode> m_CurrentPath{};
        int m_PathStep{0};
        float m_PathRefreshTimer{0.0f};
        static constexpr float k_PathRefreshInterval{0.25f};
        int m_LastHorizontalDir{0};
        bool m_IsClimbing{false};
        bool m_Start{false};
        bool m_Deserialized = false;
        std::string m_TargetName{};
        bool m_Dead{false};
    };
}
