#include "CharacterControllerComponent.h"
#include "Input/InputManager.h"
#include <SDL3/SDL.h>
#include "Commands/MoveCommand.h"
#ifdef WIN32
constexpr int GAMEPAD_DPAD_UP{0x0001};
constexpr int GAMEPAD_DPAD_DOWN{0x0002};
constexpr int GAMEPAD_DPAD_LEFT{0x0004};
constexpr int GAMEPAD_DPAD_RIGHT{0x0008};
constexpr int BUTTON_X{0x4000};
constexpr int BUTTON_A{0x1000};
constexpr int BUTTON_B{0x2000};
#else
constexpr int GAMEPAD_DPAD_UP{SDL_GAMEPAD_BUTTON_DPAD_UP};
constexpr int GAMEPAD_DPAD_DOWN{SDL_GAMEPAD_BUTTON_DPAD_DOWN};
constexpr int GAMEPAD_DPAD_LEFT{SDL_GAMEPAD_BUTTON_DPAD_LEFT};
constexpr int GAMEPAD_DPAD_RIGHT{SDL_GAMEPAD_BUTTON_DPAD_RIGHT};
constexpr int BUTTON_A{SDL_GAMEPAD_BUTTON_SOUTH};
constexpr int BUTTON_B{SDL_GAMEPAD_BUTTON_EAST};
constexpr int BUTTON_X{SDL_GAMEPAD_BUTTON_WEST};
#endif
dae::CharacterControllerComponent::CharacterControllerComponent(GameObject *go, float speed, const bool keyboard)
    : Component(go, "CharacterControllerComponent")
      , m_Keyboard{keyboard} {
    auto &Input{InputManager::GetInstance()};

    if (m_Keyboard) {
        //movement
        Input.AddBinding(Binding{KeyState::Pressed, static_cast<int>(SDL_SCANCODE_W), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, -1), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_S), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, 1), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_A), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(-1, 0), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_D), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(1, 0), speed));

        //other keys
        Input.AddBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_C), true},
                         std::make_unique<DieCommand>(m_gameObject, P1_DMG));
        Input.AddBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_Z), true},
                         std::make_unique<ScoreCommandLow>(m_gameObject, P1_BURGER_FALL));
        Input.AddBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_X), true},
                         std::make_unique<ScoreCommandHigh>(m_gameObject, P1_ENEMY_KILL));
    } else {
        //movement
        Input.AddBinding({KeyState::Pressed, GAMEPAD_DPAD_UP, false},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, -1), speed));
        Input.AddBinding({KeyState::Pressed, GAMEPAD_DPAD_DOWN, false},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, 1), speed));
        Input.AddBinding({KeyState::Pressed, GAMEPAD_DPAD_LEFT, false},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(-1, 0), speed));
        Input.AddBinding({KeyState::Pressed, GAMEPAD_DPAD_RIGHT, false},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(1, 0), speed));

        //other buttons
        Input.AddBinding({KeyState::Down, BUTTON_X, false},
                         std::make_unique<DieCommand>(m_gameObject, P2_DMG));
        Input.AddBinding({KeyState::Down, BUTTON_A, false},
                        std::make_unique<ScoreCommandLow>(m_gameObject, P2_BURGER_FALL));
        Input.AddBinding({KeyState::Down, BUTTON_B, false},
                        std::make_unique<ScoreCommandHigh>(m_gameObject, P2_ENEMY_KILL));
    }
}
