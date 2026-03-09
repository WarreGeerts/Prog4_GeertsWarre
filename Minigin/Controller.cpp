#include "Controller.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#endif

namespace dae {

    class Controller::ControllerImpl {
    public:
        XINPUT_STATE m_PrevState{};
        XINPUT_STATE m_CurrState{};
        DWORD m_ControllerIdx{0};
        WORD m_ButtonsPressedThisFrame{0};
        WORD m_ButtonsReleasedThisFrame{0};

        bool IsConnected() const {
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));
            DWORD result = XInputGetState(m_ControllerIdx, &state);
            return result == ERROR_SUCCESS;
        }

        void Update() {
            CopyMemory(&m_PrevState, &m_CurrState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_CurrState, sizeof(XINPUT_STATE));
            DWORD result = XInputGetState(m_ControllerIdx, &m_CurrState);

            if (result == ERROR_SUCCESS) {
                CalculateButtonChanges();
            } else {
                ZeroMemory(&m_CurrState, sizeof(XINPUT_STATE));
            }
        }

        void SetControllerIndex(DWORD idx) {
            m_ControllerIdx = idx;
        }

        bool IsPressed(int buttonInt) const {
            WORD button = static_cast<WORD>(buttonInt);
            return (m_CurrState.Gamepad.wButtons & button) != 0;
        }

        bool IsDownThisFrame(int buttonInt) const {
            WORD button = static_cast<WORD>(buttonInt);
            return (m_ButtonsPressedThisFrame & button) != 0;
        }

        bool IsUpThisFrame(int buttonInt) const {
            WORD button = static_cast<WORD>(buttonInt);
            return (m_ButtonsReleasedThisFrame & button) != 0;
        }

        bool IsLeftThumbRight(float deadZone = 0.2f) const {
            float lx = static_cast<float>(m_CurrState.Gamepad.sThumbLX) / 32768.0f;
            return lx > deadZone;
        }

    private:
        void CalculateButtonChanges() {
            WORD buttonChanges = m_CurrState.Gamepad.wButtons ^ m_PrevState.Gamepad.wButtons;
            m_ButtonsPressedThisFrame = buttonChanges & m_CurrState.Gamepad.wButtons;
            m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrState.Gamepad.wButtons);
        }
    };

    Controller::Controller() : m_pImpl(std::make_unique<ControllerImpl>()) {}
    Controller::~Controller() = default;

    bool Controller::IsConnected() const {
        return m_pImpl->IsConnected();
    }

    void Controller::Update() {
        m_pImpl->Update();
    }

    void Controller::SetControllerIndex(uint32_t idx) {
        m_pImpl->SetControllerIndex(idx);
    }

    bool Controller::IsPressed(int button) const {
        return m_pImpl->IsPressed(button);
    }

    bool Controller::IsDownThisFrame(int button) const {
        return m_pImpl->IsDownThisFrame(button);
    }

    bool Controller::IsUpThisFrame(int button) const {
        return m_pImpl->IsUpThisFrame(button);
    }
}
