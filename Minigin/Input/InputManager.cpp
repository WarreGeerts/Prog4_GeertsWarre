#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include <algorithm>
#include <functional>
using namespace dae;

bool InputManager::ProcessInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            return false;
        }
        if (e.type == SDL_EVENT_KEY_DOWN) {
            CheckKeyboardBindings(KeyState::Down);
        }
        if (e.type == SDL_EVENT_KEY_UP) {
            CheckKeyboardBindings(KeyState::Up);
        }
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {}
        ImGui_ImplSDL3_ProcessEvent(&e);
    }

    m_Controller.Update();

    CheckControllerBindings();
    CheckKeyboardBindings(KeyState::Pressed);

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
    for (int controllerIdx = 0; controllerIdx < 4; ++controllerIdx) {
        Controller tempController;
        tempController.SetControllerIndex(controllerIdx);
        tempController.Update();
        for (const auto &[binding, pCommand]: m_Bindings) {
            if (binding.isKeyboard || binding.controllerIdx != controllerIdx) continue;

            if (binding.state == KeyState::Pressed && m_Controller.IsPressed(binding.inputId)) {
                pCommand->Execute();
            } else if (binding.state == KeyState::Down && m_Controller.IsDownThisFrame(binding.inputId)) {
                pCommand->Execute();
            } else if (binding.state == KeyState::Up && m_Controller.IsUpThisFrame(binding.inputId)) {
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
