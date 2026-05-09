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
namespace ge {
    CharacterControllerComponent::CharacterControllerComponent(GameObject *go)
        : Component(go, "CharacterControllerComponent"){}

    CharacterControllerComponent::CharacterControllerComponent(GameObject *go, const float speed, const bool keyboard)
        : Component(go, "CharacterControllerComponent")
          , m_Keyboard{keyboard}, m_Speed(speed) {
        AddControlBindings(speed, keyboard);
    }

    void CharacterControllerComponent::InspectorGUI() {

        //Keyboard
        bool keyboard[1] = {m_Keyboard};

        if (ImGui::Checkbox("Keyboard", keyboard)) {
            m_Keyboard = keyboard[0];
        }

        //Controller index
        int controllerIdx[1] = {m_ControllerIndex};

        if (ImGui::InputInt("Controller Index", controllerIdx)) {
            m_ControllerIndex = controllerIdx[0];
        }

        //speed
        float speed[1] = {m_Speed};

        if (ImGui::InputFloat("Movement Speed", speed)) {
            m_Speed = speed[0];
        }
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
        } else {
            //movement
            Input.AddBinding({KeyState::Pressed, GAMEPAD_DPAD_UP, false, m_ControllerIndex},
                             std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, -1), speed));
            Input.AddBinding({KeyState::Pressed, GAMEPAD_DPAD_DOWN, false, m_ControllerIndex},
                             std::make_unique<MoveCommand>(m_gameObject, glm::vec2(0, 1), speed));
            Input.AddBinding({KeyState::Pressed, GAMEPAD_DPAD_LEFT, false, m_ControllerIndex},
                             std::make_unique<MoveCommand>(m_gameObject, glm::vec2(-1, 0), speed));
            Input.AddBinding({KeyState::Pressed, GAMEPAD_DPAD_RIGHT, false, m_ControllerIndex},
                             std::make_unique<MoveCommand>(m_gameObject, glm::vec2(1, 0), speed));
        }
        m_Bound = true;
    }

    void CharacterControllerComponent::RemoveControlBindings() {
        if (!m_Bound) return;
        auto &Input{InputManager::GetInstance()};

        if (m_Keyboard) {
            //movement
            Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_W), true});
            Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_S), true});
            Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_A), true});
            Input.RemoveBinding({KeyState::Pressed, static_cast<int>(SDL_SCANCODE_D), true});
        } else {
            //movement
            Input.RemoveBinding({KeyState::Pressed, GAMEPAD_DPAD_UP, false, m_ControllerIndex});
            Input.RemoveBinding({KeyState::Pressed, GAMEPAD_DPAD_DOWN, false, m_ControllerIndex});
            Input.RemoveBinding({KeyState::Pressed, GAMEPAD_DPAD_LEFT, false, m_ControllerIndex});
            Input.RemoveBinding({KeyState::Pressed, GAMEPAD_DPAD_RIGHT, false, m_ControllerIndex});
        }

        m_Bound = false;
    }

    nlohmann::ordered_json CharacterControllerComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["keyboard"] = m_Keyboard;
        data["controller_index"] = m_ControllerIndex;
        data["speed"] = m_Speed;
        return data;
    }

    void CharacterControllerComponent::Deserialize(const nlohmann::ordered_json &data) {
        RemoveControlBindings();
        m_Keyboard = data.value("keyboard", true);
        m_ControllerIndex = data.value("controller_index", 0);
        m_Speed = data.value("speed", 50.f);
        AddControlBindings(m_Speed, m_Keyboard);
    }
}
