#include "SceneSwitchComponent.h"
#include <SDL3/SDL.h>
#include "../Commands/GameCommands.h"
#include "Input/InputManager.h"
//buttons
#ifdef WIN32
constexpr int BUTTON_A{0x1000};
#else
constexpr int BUTTON_A{SDL_GAMEPAD_BUTTON_SOUTH};
#endif
namespace dae {
    SceneSwitchComponent::SceneSwitchComponent(GameObject *Go)
        : Component(Go, "SceneSwitchComponent") {
        AddControlBindings();
    }

    void SceneSwitchComponent::Update() {
    }

    void SceneSwitchComponent::InspectorGUI() {
    }

    nlohmann::ordered_json SceneSwitchComponent::Serialize() const {
        nlohmann::ordered_json data;

        return data;
    }

    void SceneSwitchComponent::Deserialize(const nlohmann::ordered_json&) {
        RemoveControlBindings();

        AddControlBindings();
    }

    void SceneSwitchComponent::RemoveControlBindings() {
        if (!m_Bound) return;
        auto &Input{InputManager::GetInstance()};

        Input.RemoveBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_A), true});
        Input.RemoveBinding({KeyState::Down, BUTTON_A, false});

        m_Bound = false;
    }

    void SceneSwitchComponent::AddControlBindings() {
        if (m_Bound) return;
        auto &Input{InputManager::GetInstance()};

        Input.AddBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_A), true},
                         std::make_unique<GoToLobbyCommand>(EventRegistry::GO_TO_LOBBY));
        Input.AddBinding({KeyState::Pressed, BUTTON_A, false},
                         std::make_unique<GoToLobbyCommand>(EventRegistry::GO_TO_LOBBY));

        m_Bound = true;
    };
}
