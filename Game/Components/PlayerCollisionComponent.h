#pragma once
#include "glm/vec2.hpp"
#include "Components/Component.h"

namespace ge {
    class ColliderComponent;
}

namespace game {
    class CollisionGridComponent;

    class PlayerCollisionComponent final : public ge::Component {
    public:
        explicit PlayerCollisionComponent(ge::GameObject *go);
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
    private:
        ge::ColliderComponent* m_Collider{nullptr};
        CollisionGridComponent* m_Grid{nullptr};

        [[nodiscard]] glm::vec2 getPivotPoint() const;
    };
}
