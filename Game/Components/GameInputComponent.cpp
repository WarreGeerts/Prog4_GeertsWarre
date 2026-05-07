#include "GameInputComponent.h"
#include "../Commands/GameCommands.h"
#include "Input/InputManager.h"
#include <SDL3/SDL.h>

namespace dae {
    GameInputComponent::GameInputComponent(GameObject *Go)
        : Component(Go, "GameInputComponent") {
        AddControlBindings();
    }

    void GameInputComponent::Update() {}
    void GameInputComponent::InspectorGUI() {}

    void GameInputComponent::AddControlBindings() {
        if (m_Bound) return;
        auto &Input{InputManager::GetInstance()};

        Input.AddBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_A), true},
                         std::make_unique<GoToLobbyCommand>(EventRegistry::GO_TO_LOBBY));

        m_Bound = true;
    }

    void GameInputComponent::RemoveControlBindings() {
        if (!m_Bound) return;
        auto &Input{InputManager::GetInstance()};

        Input.RemoveBinding({KeyState::Down, static_cast<int>(SDL_SCANCODE_A), true});

        m_Bound = false;
    }

    void GameInputComponent::Deserialize(const nlohmann::ordered_json &) {
        RemoveControlBindings();
        AddControlBindings();
    }
}
