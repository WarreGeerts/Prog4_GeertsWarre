#include "Controller.h"
using namespace dae;

bool Controller::IsConnected() const {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    const DWORD dwResult{XInputGetState(m_ControllerIdx, &state)};
    return dwResult == ERROR_SUCCESS;
}

void Controller::Update() {
    CopyMemory(&m_PrevState, &m_CurrState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrState, sizeof(XINPUT_STATE));
    DWORD result = XInputGetState(m_ControllerIdx, &m_CurrState);

    if (result == ERROR_SUCCESS) {
        CalculateButtonChanges();
    } else {
        ZeroMemory(&m_CurrState, sizeof(XINPUT_STATE));
    }}

void Controller::CalculateButtonChanges() {
    const WORD buttonChanges = m_CurrState.Gamepad.wButtons ^ m_PrevState.Gamepad.wButtons;
    m_ButtonsPressedThisFrame = buttonChanges & m_CurrState.Gamepad.wButtons;
    m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrState.Gamepad.wButtons);
}

bool Controller::IsLeftThumbRight(const float deadZone = 0.2f) const {
    const float lx = static_cast<float>(m_CurrState.Gamepad.sThumbLX) / 32768.0f;
    return lx > deadZone;
}
