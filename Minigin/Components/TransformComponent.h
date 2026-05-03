#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae {
    class TransformComponent final : public Component {
    public:
        explicit TransformComponent(GameObject *go) : Component(go,"TransformComponent"), m_position(0, 0, 0) {}
        TransformComponent(GameObject *go, const float x, const float y) : Component(go,"TransformComponent"), m_position(x, y, 0) {}
        void Update() override {}
        void Render() const override {}
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

        [[nodiscard]] const glm::vec3 &GetPosition() const;
        void SetPosition(float x, float y, float z = 0);
        void SetPosition(const glm::vec3 &position);

    private:
        glm::vec3 m_position;
    };
}
