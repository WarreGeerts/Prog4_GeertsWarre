#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include <algorithm>
#include <functional>

namespace ge {
    bool Binding::operator==(const Binding &other) const {
        return state == other.state &&
               inputId == other.inputId &&
               isKeyboard == other.isKeyboard &&
               controllerIdx == other.controllerIdx;
    }

    bool InputManager::ProcessInput() {
        SDL_Event e;
        const ImGuiIO &io = ImGui::GetIO();

        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);

            if (e.type == SDL_EVENT_QUIT) {
                return false;
            }

            if (!io.WantCaptureKeyboard) {
                if (e.type == SDL_EVENT_KEY_DOWN) {
                    CheckKeyboardBindings(KeyState::Down);
                }
                if (e.type == SDL_EVENT_KEY_UP) {
                    CheckKeyboardBindings(KeyState::Up);
                }
            }

            if (!io.WantCaptureMouse) {
                if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {}
            }
        }

        for (const auto &controller: m_Controllers) {
            controller->Update();
        }
        CheckControllerBindings();

        if (!io.WantCaptureKeyboard) {
            CheckKeyboardBindings(KeyState::Pressed);
        }

        return true;
    }

    void InputManager::AddBinding(const Binding &binding, std::unique_ptr<Command> pCommand) {
        m_Bindings.emplace_back(binding, std::move(pCommand));
    }

    void InputManager::RemoveBinding(const Binding &binding) {
        m_Bindings.erase(
            std::remove_if(m_Bindings.begin(), m_Bindings.end(),
                           [&](const auto &pair) {
                               return pair.first == binding;
                           }),
            m_Bindings.end()
        );
    }

    void InputManager::ClearBindings() {
        m_Bindings.clear();
    }

    void InputManager::CheckControllerBindings() {
        for (size_t i = 0; i < m_Controllers.size(); ++i) {
            const auto &controller = m_Controllers[i];
            if (!controller->IsConnected()) continue;

            for (const auto &[binding, pCommand]: m_Bindings) {
                if (binding.isKeyboard || binding.controllerIdx != static_cast<int>(i)) continue;

                if (binding.state == KeyState::Pressed && controller->IsPressed(binding.inputId)) {
                    pCommand->Execute();
                } else if (binding.state == KeyState::Down && controller->IsDownThisFrame(binding.inputId)) {
                    pCommand->Execute();
                } else if (binding.state == KeyState::Up && controller->IsUpThisFrame(binding.inputId)) {
                    pCommand->Execute();
                }
            }
        }
    }

    void InputManager::CheckKeyboardBindings(const KeyState state) {
        UpdateKeyboardState();

        for (const auto &[binding, pCommand]: m_Bindings) {
            if (!binding.isKeyboard) continue;

            bool isActive{false};
            if (binding.state == state) {
                isActive = m_KeyboardState[binding.inputId] != 0;
            }

            if (isActive) {
                pCommand->Execute();
            }
        }
    }

    void InputManager::UpdateKeyboardState() {
        m_KeyboardState = SDL_GetKeyboardState(nullptr);
    }

    void InputManager::AddController(uint32_t idx) {
        m_Controllers.push_back(std::make_unique<Controller>(idx));
    }
}
