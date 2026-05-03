#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae {
    class RotateComponent final : public Component {
    public:
        explicit RotateComponent(GameObject *go) : Component(go,"RotateComponent") {}

        RotateComponent(GameObject *go, const float radius, const float angularSpeed)
            : Component(go,"RotateComponent"), m_Radius{radius}, m_AngularSpeed{angularSpeed}, m_CurrentAngle{0.f} {}

        void Update() override;
        void Render() const override {}
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

        void InspectorGUI() override;

        void SetRadius(const float radius) { m_Radius = radius; }
        void SetAngularSpeed(const float angularSpeed) { m_AngularSpeed = angularSpeed; }

    private:
        float m_Radius{100.f};
        float m_AngularSpeed{1.f}; //in rad/s, so 2*pi = 1 rotation (360 degrees)
        float m_CurrentAngle{0.f};

        const float m_PI = 3.14159265359f;
    };
}
