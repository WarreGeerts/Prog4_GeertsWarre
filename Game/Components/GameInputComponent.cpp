#include "GameInputComponent.h"
#include "../Commands/GameCommands.h"
#include "Input/InputManager.h"
#include <SDL3/SDL.h>

#include "../Events.h"
#ifdef WIN32
constexpr int GAMEPAD_DPAD_UP{0x0001};
constexpr int GAMEPAD_DPAD_DOWN{0x0002};
constexpr int BUTTON_A{0x1000};
#else
constexpr int GAMEPAD_DPAD_UP{SDL_GAMEPAD_BUTTON_DPAD_UP};
constexpr int GAMEPAD_DPAD_DOWN{SDL_GAMEPAD_BUTTON_DPAD_DOWN};
constexpr int BUTTON_A{SDL_GAMEPAD_BUTTON_SOUTH};
#endif

namespace game {
    GameInputComponent::GameInputComponent(ge::GameObject *Go)
        : Component(Go, "GameInputComponent") {
        AddControlBindings();
    }

    void GameInputComponent::Update() {}
    void GameInputComponent::InspectorGUI() {}

    void GameInputComponent::AddControlBindings() {
        if (m_Bound) return;
        auto &Input{ge::InputManager::GetInstance()};

        //keyboard
        Input.AddBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_A), true},
                         std::make_unique<SelectionCommand>(EventRegistry::A_BUTTON_PRESSED));
        Input.AddBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_W), true},
                         std::make_unique<UISelectionDownCommand>(EventRegistry::UI_SELECTION_DOWN));
        Input.AddBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_S), true},
                        std::make_unique<UISelectionUpCommand>(EventRegistry::UI_SELECTION_UP));

        //controller
        Input.AddBinding({ge::KeyState::Down, BUTTON_A, false},
                         std::make_unique<SelectionCommand>(EventRegistry::A_BUTTON_PRESSED));
        Input.AddBinding({ge::KeyState::Down, GAMEPAD_DPAD_UP, false},
                         std::make_unique<UISelectionDownCommand>(EventRegistry::UI_SELECTION_DOWN));
        Input.AddBinding({ge::KeyState::Down, GAMEPAD_DPAD_DOWN, false},
                        std::make_unique<UISelectionUpCommand>(EventRegistry::UI_SELECTION_UP));

        m_Bound = true;
    }

    void GameInputComponent::RemoveControlBindings() {
        if (!m_Bound) return;
        auto &Input{ge::InputManager::GetInstance()};

        //keyboard
        Input.RemoveBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_A), true});
        Input.RemoveBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_W), true});
        Input.RemoveBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_S), true});

        //controller

        Input.RemoveBinding({ge::KeyState::Down, static_cast<int>(BUTTON_A), false});
        Input.RemoveBinding({ge::KeyState::Down, static_cast<int>(GAMEPAD_DPAD_UP), false});
        Input.RemoveBinding({ge::KeyState::Down, static_cast<int>(GAMEPAD_DPAD_DOWN), false});

        m_Bound = false;
    }

    void GameInputComponent::Deserialize(const nlohmann::ordered_json &) {
        RemoveControlBindings();
        AddControlBindings();
    }
}
