#pragma once
#include <memory>
#include <utility>
#include <vector>
#include "Commands/Command.h"
#include "Controller.h"
#include "Singletons/Singleton.h"

namespace dae {
    enum class KeyState { Pressed, Down, Up };

    using Key = int;
    using Button = int;

    struct Binding {
        KeyState state{};
        int inputId{};
        bool isKeyboard{};
        int controllerIdx{0};
        bool operator==(const Binding &other) const;
    };

    class InputManager final : public Singleton<InputManager> {
    public:
        bool ProcessInput();
        void AddBinding(const Binding &binding, std::unique_ptr<Command> pCommand);
        void RemoveBinding(const Binding &binding);
        void ClearBindings();

    private:
        std::vector<std::pair<Binding, std::unique_ptr<Command> > > m_Bindings;
        std::vector<std::unique_ptr<Controller> > m_Controllers;
        const bool *m_KeyboardState{nullptr};
        void CheckControllerBindings();
        void CheckKeyboardBindings(KeyState state);
        void UpdateKeyboardState();
        void AddController(uint32_t idx);
    };
}
