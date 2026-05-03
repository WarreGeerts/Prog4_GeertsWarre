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
//TODO:: Make comprehensive guide on how to do this, like being able to add commands, add keybinds, look for commands/keybinds out of a list
//TODO:: Visualise button/keyboard presses
//TODO:: When in ImGui disable button presses until focused on gameScreen again
using namespace dae;

CharacterControllerComponent::CharacterControllerComponent(GameObject *go)
    : Component(go, "CharacterControllerComponent")
      , m_Keyboard(true) {}

CharacterControllerComponent::CharacterControllerComponent(GameObject *go, const float speed, const bool keyboard)
    : Component(go, "CharacterControllerComponent")
      , m_Keyboard{keyboard}, m_Speed(speed) {
    AddControlBindings(speed, keyboard);
}

void CharacterControllerComponent::InspectorGUI() {
    //todo: do some shit in here
    //speed
    //controls?
    //keyboard or not
}

void CharacterControllerComponent::AddControlBindings(float speed, bool keyboard) {
    if (m_Bound) return;
    auto &Input{InputManager::GetInstance()};
    if (m_Keyboard) {
        //movement
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_W), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, -1), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_S), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, 1), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_A), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(-1, 0), speed));
        Input.AddBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_D), true},
                         std::make_unique<MoveCommand>(m_gameObject, glm::vec2(1, 0), speed));

        //other keys
        Input.AddBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_C), true},
                         std::make_unique<DieCommand>(m_gameObject, EventRegistry::P1_DMG));
        Input.AddBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_Z), true},
                         std::make_unique<ScoreCommandLow>(m_gameObject, EventRegistry::P1_BURGER_FALL));
        Input.AddBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_X), true},
                         std::make_unique<ScoreCommandHigh>(m_gameObject, EventRegistry::P1_ENEMY_KILL));
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
                         std::make_unique<DieCommand>(m_gameObject, EventRegistry::P2_DMG));
        Input.AddBinding({KeyState::Down, BUTTON_A, false},
                         std::make_unique<ScoreCommandLow>(m_gameObject, EventRegistry::P2_BURGER_FALL));
        Input.AddBinding({KeyState::Down, BUTTON_B, false},
                         std::make_unique<ScoreCommandHigh>(m_gameObject, EventRegistry::P2_ENEMY_KILL));
    }
    m_Bound = true;
}

void CharacterControllerComponent::RemoveControlBindings() {
    if (!m_Bound) return;
    auto &Input{InputManager::GetInstance()};
    Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_W), true});

    if (m_Keyboard) {
        //movement
        Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_W), true});
        Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_S), true});
        Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_A), true});
        Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_D), true});

        //other keys
        Input.RemoveBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_C), true});
        Input.RemoveBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_Z), true});
        Input.RemoveBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_X), true});
    } else {
        //movement
        Input.RemoveBinding({KeyState::Pressed, GAMEPAD_DPAD_UP, false});
        Input.RemoveBinding({KeyState::Pressed, GAMEPAD_DPAD_DOWN, false});
        Input.RemoveBinding({KeyState::Pressed, GAMEPAD_DPAD_LEFT, false});
        Input.RemoveBinding({KeyState::Pressed, GAMEPAD_DPAD_RIGHT, false});

        //other buttons
        Input.RemoveBinding({KeyState::Down, BUTTON_X, false});
        Input.RemoveBinding({KeyState::Down, BUTTON_A, false});
        Input.RemoveBinding({KeyState::Down, BUTTON_B, false});
    }

    m_Bound = false;
}

nlohmann::ordered_json CharacterControllerComponent::Serialize() const {
    nlohmann::ordered_json data;
    data["keyboard"] = m_Keyboard;
    data["speed"] = m_Speed;
    return data;
}

void CharacterControllerComponent::Deserialize(const nlohmann::ordered_json &data) {
    RemoveControlBindings();
    m_Keyboard = data.value("keyboard", true);
    m_Speed = data.value("speed", 50.f);
    AddControlBindings(m_Speed, m_Keyboard);
}
