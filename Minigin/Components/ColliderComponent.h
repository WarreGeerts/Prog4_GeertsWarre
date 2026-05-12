#pragma once
#include "Component.h"
#include "GameObject.h"

namespace ge {

    class ColliderComponent final : public Component {
    public:
        explicit ColliderComponent(GameObject *go);
        void Update() override{};
        void Render() const override;
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override{return {};};
        void Deserialize(const nlohmann::ordered_json &data) override;

        [[nodiscard]] glm::vec4 GetWorldBounds() const;
        [[nodiscard]] bool IsColliding(float worldX, float worldY) const;

    private:
        glm::vec2 m_Offset{};
        float m_Width{};
        float m_Height{};
        bool m_Debug{false};
    };
}
