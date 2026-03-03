#pragma once
#include "Singleton.h"

namespace dae {
    class InputManager final : public Singleton<InputManager> {
    public:
        bool ProcessInput();
        [[nodiscard]] bool IsF1Pressed() const { return m_F1Pressed; };

    private:
        bool m_F1Pressed{false};
    };
}
