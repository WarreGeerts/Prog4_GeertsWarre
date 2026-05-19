#include "PlayerStateControllerComponent.h"
#include "Commands/MoveCommand.h"
#include "Input/InputManager.h"
#include "States/PSIdle.h"
#include <SDL3/SDL.h>
#ifdef WIN32
constexpr int GAMEPAD_DPAD_UP{0x0001};
constexpr int GAMEPAD_DPAD_DOWN{0x0002};
constexpr int GAMEPAD_DPAD_LEFT{0x0004};
constexpr int GAMEPAD_DPAD_RIGHT{0x0008};
constexpr int BUTTON_X{0x4000};
/*constexpr int BUTTON_A{0x1000};
constexpr int BUTTON_B{0x2000};*/
#else
constexpr int GAMEPAD_DPAD_UP{SDL_GAMEPAD_BUTTON_DPAD_UP};
constexpr int GAMEPAD_DPAD_DOWN{SDL_GAMEPAD_BUTTON_DPAD_DOWN};
constexpr int GAMEPAD_DPAD_LEFT{SDL_GAMEPAD_BUTTON_DPAD_LEFT};
constexpr int GAMEPAD_DPAD_RIGHT{SDL_GAMEPAD_BUTTON_DPAD_RIGHT};
constexpr int BUTTON_X{SDL_GAMEPAD_BUTTON_WEST};
/*constexpr int BUTTON_A{SDL_GAMEPAD_BUTTON_SOUTH};
constexpr int BUTTON_B{SDL_GAMEPAD_BUTTON_EAST};*/
#endif
namespace game {
    PlayerStateControllerComponent::PlayerStateControllerComponent(ge::GameObject *go)
        : Component(go, "PlayerStateControllerComponent") {
        SetState(std::make_unique<PSIdle>());
    }

    PlayerStateControllerComponent::PlayerStateControllerComponent(ge::GameObject *go, float speed, bool useKeyboard)
        : Component(go, "PlayerStateControllerComponent"), m_Speed(speed), m_Keyboard(useKeyboard) {
        AddControlBindings();
        SetState(std::make_unique<PSIdle>());
    }

    void PlayerStateControllerComponent::Update() {
        if (m_pCurrentState) {
            m_pCurrentState->OnUpdate(this);
        }
    }

    void PlayerStateControllerComponent::LateUpdate() {
        m_InputDirection = glm::vec2(0, 0);
    }

    void PlayerStateControllerComponent::AddInputDirection(const glm::vec2 &direction) {
        m_InputDirection += direction;
    }

    void PlayerStateControllerComponent::AddControlBindings() {
        if (m_Bound) return;
        auto &Input{ge::InputManager::GetInstance()};
        if (m_Keyboard) {
            //movement
            Input.AddBinding({ge::KeyState::Pressed, static_cast<int>(SDL_SCANCODE_W), true},
                             std::make_unique<ge::MoveCommand>(m_gameObject, glm::vec2(0, -1), m_Speed));
            Input.AddBinding({ge::KeyState::Pressed, static_cast<int>(SDL_SCANCODE_S), true},
                             std::make_unique<ge::MoveCommand>(m_gameObject, glm::vec2(0, 1), m_Speed));
            Input.AddBinding({ge::KeyState::Pressed, static_cast<int>(SDL_SCANCODE_A), true},
                             std::make_unique<ge::MoveCommand>(m_gameObject, glm::vec2(-1, 0), m_Speed));
            Input.AddBinding({ge::KeyState::Pressed, static_cast<int>(SDL_SCANCODE_D), true},
                             std::make_unique<ge::MoveCommand>(m_gameObject, glm::vec2(1, 0), m_Speed));
        } else {
            //movement
            Input.AddBinding({ge::KeyState::Pressed, GAMEPAD_DPAD_UP, false, m_ControllerIndex},
                             std::make_unique<ge::MoveCommand>(m_gameObject, glm::vec2(0, -1), m_Speed));
            Input.AddBinding({ge::KeyState::Pressed, GAMEPAD_DPAD_DOWN, false, m_ControllerIndex},
                             std::make_unique<ge::MoveCommand>(m_gameObject, glm::vec2(0, 1), m_Speed));
            Input.AddBinding({ge::KeyState::Pressed, GAMEPAD_DPAD_LEFT, false, m_ControllerIndex},
                             std::make_unique<ge::MoveCommand>(m_gameObject, glm::vec2(-1, 0), m_Speed));
            Input.AddBinding({ge::KeyState::Pressed, GAMEPAD_DPAD_RIGHT, false, m_ControllerIndex},
                             std::make_unique<ge::MoveCommand>(m_gameObject, glm::vec2(1, 0), m_Speed));
        }
        m_Bound = true;
    }

    void PlayerStateControllerComponent::RemoveControlBindings() {
        if (!m_Bound) return;
        auto &Input{ge::InputManager::GetInstance()};

        if (m_Keyboard) {
            //movement
            Input.RemoveBinding({ge::KeyState::Pressed, static_cast<int>(SDL_SCANCODE_W), true});
            Input.RemoveBinding({ge::KeyState::Pressed, static_cast<int>(SDL_SCANCODE_S), true});
            Input.RemoveBinding({ge::KeyState::Pressed, static_cast<int>(SDL_SCANCODE_A), true});
            Input.RemoveBinding({ge::KeyState::Pressed, static_cast<int>(SDL_SCANCODE_D), true});
        } else {
            //movement
            Input.RemoveBinding({ge::KeyState::Pressed, GAMEPAD_DPAD_UP, false, m_ControllerIndex});
            Input.RemoveBinding({ge::KeyState::Pressed, GAMEPAD_DPAD_DOWN, false, m_ControllerIndex});
            Input.RemoveBinding({ge::KeyState::Pressed, GAMEPAD_DPAD_LEFT, false, m_ControllerIndex});
            Input.RemoveBinding({ge::KeyState::Pressed, GAMEPAD_DPAD_RIGHT, false, m_ControllerIndex});
        }

        m_Bound = false;
    }

    void PlayerStateControllerComponent::SetState(std::unique_ptr<PlayerState> newState) {
        if (m_pCurrentState) m_pCurrentState->OnExit(this);
        m_pCurrentState = std::move(newState);
        if (m_pCurrentState) m_pCurrentState->OnEnter(this);
    }
}
