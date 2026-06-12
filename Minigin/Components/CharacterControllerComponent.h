#pragma once
#include "Component.h"
#include "glm/vec2.hpp"

namespace ge {
    class CharacterControllerComponent final : public Component {
    public:
        explicit CharacterControllerComponent(GameObject *go);
        CharacterControllerComponent(GameObject *go, float speed, bool keyboard);
        ~CharacterControllerComponent() override = default;
        void Update() override {};
        void InspectorGUI() override;
        void Render() const override {};
        void SetInputDirection(const glm::vec2 &dir) { m_InputDirection = dir; }
        [[nodiscard]] glm::vec2 GetInputDirection() const { return m_InputDirection; }
        void PostUpdate() { m_InputDirection = glm::vec2(0, 0); }
        void AddControlBindings(float speed, bool keyboard);
        void RemoveControlBindings();
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        int GetControllerIndex() const { return m_ControllerIndex; }

    private:
        bool m_Keyboard{true};
        float m_Speed{50.f};
        bool m_Bound{false};
        int m_ControllerIndex{0};
        glm::vec2 m_InputDirection{0, 0};
    };
}
