#pragma once
#include <XInput.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace dae {
    class Controller {
    public:
        bool IsConnected() const;
        void Update();
        [[nodiscard]] bool IsPressed(const WORD& button) const {return m_CurrState.Gamepad.wButtons & button;}
        [[nodiscard]] bool IsDownThisFrame(const WORD& button) const {return m_ButtonsPressedThisFrame & button;}
        [[nodiscard]] bool IsUpThisFrame(const WORD& button) const {return m_ButtonsReleasedThisFrame & button;}

    private:
        XINPUT_STATE m_PrevState{};
        XINPUT_STATE m_CurrState{};
        DWORD m_ControllerIdx{0};
        WORD m_ButtonsPressedThisFrame{0};
        WORD m_ButtonsReleasedThisFrame{0};

        void CalculateButtonChanges();
        [[nodiscard]] bool IsLeftThumbRight(float deadZone) const;
    };
}
