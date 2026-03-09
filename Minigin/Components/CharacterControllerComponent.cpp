#include "CharacterControllerComponent.h"
#include "InputManager.h"
#include <SDL3/SDL.h>
#include "../Commands/MoveCommand.h"

constexpr int XINPUT_GAMEPAD_DPAD_UP    = 0x0001;
constexpr int XINPUT_GAMEPAD_DPAD_DOWN  = 0x0002;
constexpr int XINPUT_GAMEPAD_DPAD_LEFT  = 0x0004;
constexpr int XINPUT_GAMEPAD_DPAD_RIGHT = 0x0008;

dae::CharacterControllerComponent::CharacterControllerComponent(GameObject *go, float speed, const bool keyboard,
                                                                const int controllerIdx)
    : Component(go)
      , m_Keyboard{keyboard}
      , m_ControllerIdx{controllerIdx} { //TODO make sure it works with multiple controllers that's why idx
    auto &Input{InputManager::GetInstance()};

    if (m_Keyboard) {
        Input.AddBinding(Binding{KeyState::Pressed, static_cast<int>(SDL_SCANCODE_W), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, -1), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_S), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, 1), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_A), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(-1, 0), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_D), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(1, 0), speed));
    } else {
        Input.AddBinding({KeyState::Pressed, XINPUT_GAMEPAD_DPAD_UP, false},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, -1), speed));
        Input.AddBinding({KeyState::Pressed, XINPUT_GAMEPAD_DPAD_DOWN, false},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, 1), speed));
        Input.AddBinding({KeyState::Pressed, XINPUT_GAMEPAD_DPAD_LEFT, false},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(-1, 0), speed));
        Input.AddBinding({KeyState::Pressed, XINPUT_GAMEPAD_DPAD_RIGHT, false},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(1, 0), speed));
    }
}
