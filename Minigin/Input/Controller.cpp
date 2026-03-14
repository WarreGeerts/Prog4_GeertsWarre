#include "Controller.h"
#include <SDL3/SDL.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>

namespace dae {
    class Controller::ControllerImpl {
        DWORD m_ControllerIndex{0};
        XINPUT_STATE m_PreviousState{};
        XINPUT_STATE m_CurrentState{};
        WORD m_ButtonsPressedThisFrame{};
        WORD m_ButtonsReleasedThisFrame{};

    public:
        explicit ControllerImpl(const uint32_t controllerIndex) : m_ControllerIndex(controllerIndex) {
            ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        }

        void Update() {
            CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            DWORD result{XInputGetState(m_ControllerIndex, &m_CurrentState)};

            if (result == ERROR_SUCCESS) {
                const WORD buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
                m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
                m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
            }
        }

        [[nodiscard]] bool IsConnected() const {
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));
            return XInputGetState(m_ControllerIndex, &state) == ERROR_SUCCESS;
        }

        void SetControllerIndex(const uint32_t idx) { m_ControllerIndex = static_cast<DWORD>(idx); }

        [[nodiscard]] bool IsPressed(const int button) const {
            return m_CurrentState.Gamepad.wButtons & static_cast<WORD>(button);
        }

        [[nodiscard]] bool IsDownThisFrame(const int button) const {
            return m_ButtonsPressedThisFrame & static_cast<WORD>(button);
        }

        [[nodiscard]] bool IsUpThisFrame(const int button) const {
            return m_ButtonsReleasedThisFrame & static_cast<WORD>(button);
        }
    };
}
#else
namespace dae {
    class Controller::ControllerImpl {
        int m_ControllerIndex{0};
        SDL_Gamepad *m_pController{nullptr};
        uint32_t m_PreviousState{0};
        uint32_t m_CurrentState{0};
        uint32_t m_ButtonsPressedThisFrame{0};
        uint32_t m_ButtonsReleasedThisFrame{0};

    public:
        explicit ControllerImpl(uint32_t controllerIndex) : m_ControllerIndex(controllerIndex) {
            UpdateConnection();
        }

        ~ControllerImpl() {
            if (m_pController) {
                SDL_CloseGamepad(m_pController);
                m_pController = nullptr;
            }
        }

        void Update() {
            m_PreviousState = m_CurrentState;
            m_CurrentState = 0;

            UpdateConnection();

            if (!m_pController) return;

            for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i) {
                if (SDL_GetGamepadButton(m_pController, static_cast<SDL_GamepadButton>(i))) {
                    m_CurrentState |= (1u << i);
                }
            }

            const uint32_t buttonChanges = m_CurrentState ^ m_PreviousState;
            m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState;
            m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState);
        }

        [[nodiscard]] bool IsConnected() const { return m_pController != nullptr; }

        void SetControllerIndex(uint32_t idx) {
            m_ControllerIndex = idx;
            if (m_pController) {
                SDL_CloseGamepad(m_pController);
                m_pController = nullptr;
            }
            UpdateConnection();
        }

        [[nodiscard]] bool IsPressed(const int button) const { return m_CurrentState & (1u << button); }
        [[nodiscard]] bool IsDownThisFrame(const int button) const { return m_ButtonsPressedThisFrame & (1u << button); }
        [[nodiscard]] bool IsUpThisFrame(const int button) const { return m_ButtonsReleasedThisFrame & (1u << button); }

    private:
        void UpdateConnection() {
            if (m_pController) return;

            SDL_UpdateGamepads();

            int numControllers = 0;
            SDL_JoystickID *gamepadIDs = SDL_GetGamepads(&numControllers);
            if (gamepadIDs && numControllers > 0 && m_ControllerIndex < numControllers) {
                m_pController = SDL_OpenGamepad(gamepadIDs[m_ControllerIndex]);
            }

            if (gamepadIDs) {
                SDL_free(gamepadIDs);
            }
        }
    };
}
#endif

namespace dae {
    Controller::Controller() : m_pImpl(std::make_unique<ControllerImpl>(0)) {}
    Controller::~Controller() = default;
    bool Controller::IsConnected() const { return m_pImpl->IsConnected(); }
    void Controller::Update() { m_pImpl->Update(); }
    void Controller::SetControllerIndex(const uint32_t idx) const { m_pImpl->SetControllerIndex(idx); }
    bool Controller::IsPressed(const int button) const { return m_pImpl->IsPressed(button); }
    bool Controller::IsDownThisFrame(const int button) const { return m_pImpl->IsDownThisFrame(button); }
    bool Controller::IsUpThisFrame(const int button) const { return m_pImpl->IsUpThisFrame(button); }
}
