#pragma once
#include <memory>
#include <utility>
#include <vector>
#include <SDL3/SDL.h>
#include "Commands/Command.h"
#include "Controller.h"
#include "Singleton.h"

namespace dae {

    enum class KeyState{Pressed,Down,Up};
    using Key = int;
    using Button = WORD;

    struct Binding {
        KeyState state;
        int inputId;
        bool isKeyboard;

        bool operator==(const Binding& other) const {
            return state == other.state &&
                   inputId == other.inputId &&
                   isKeyboard == other.isKeyboard;
        }
    };

    class InputManager final : public Singleton<InputManager> {
    public:
        bool ProcessInput();

        void AddBinding(const Binding& binding, std::unique_ptr<Command> pCommand);
        void RemoveBinding(const Binding& binding);

    private:
        std::vector<std::pair<Binding, std::unique_ptr<Command>>> m_Bindings;
        Controller m_Controller;
        const bool *m_KeyboardState{nullptr};
        Uint8* m_PrevKeyboardState{nullptr};

        void CheckControllerBindings();
        void CheckKeyboardBindings(KeyState state);
        void UpdateKeyboardState();
    };
}
