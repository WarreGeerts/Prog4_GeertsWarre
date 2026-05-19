#pragma once
#include "Components/Component.h"
#include "glm/vec2.hpp"
#include "States/PlayerState.h"

namespace game {
    class PlayerStateControllerComponent final : public ge::Component {
    public:
        explicit PlayerStateControllerComponent(ge::GameObject *go);
        PlayerStateControllerComponent(ge::GameObject *go, float speed, bool useKeyboard);
        ~PlayerStateControllerComponent() override = default;
        void Update() override;
        void LateUpdate();
        void SetState(std::unique_ptr<PlayerState> newState);
        void AddInputDirection(const glm::vec2 &direction);
        [[nodiscard]] glm::vec2 GetInputDirection() const { return m_InputDirection; }
        [[nodiscard]] float GetSpeed() const { return m_Speed; }
        [[nodiscard]] bool IsOverlappingLadder() const { return m_IsOverlappingLadder; }
        // Toggle this via your collision system
        [[nodiscard]] ge::GameObject *GetOwner() const { return m_gameObject; }
        // Gives states access to the GameObject!
        void Render() const override {};
        void InspectorGUI() override {};
        [[nodiscard]] nlohmann::ordered_json Serialize() const override { return {}; };
        void Deserialize(const nlohmann::ordered_json &data) override { (void) data; };

    private:
        void AddControlBindings();
        void RemoveControlBindings();
        std::unique_ptr<PlayerState> m_pCurrentState{nullptr};
        glm::vec2 m_InputDirection{0, 0};
        float m_Speed{50.f};
        bool m_Keyboard{true};
        bool m_Bound{false};
        int m_ControllerIndex{0};
        bool m_IsOverlappingLadder{false};
    };
}
