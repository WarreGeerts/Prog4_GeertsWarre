#include "GMSManager.h"

#include "GMSSinglePlayer.h"


namespace game {
    void GMSManager::Initialize() {
    }

    void GMSManager::ChangeState(std::unique_ptr<GameModeState> newState) {
        if (m_CurrentGameState) {
            m_CurrentGameState->OnExit();
        }

        m_CurrentGameState = std::move(newState);
        m_CurrentGameState->OnEnter();
    }

    void GMSManager::Update() {
        if (m_CurrentGameState) m_CurrentGameState->Update();
    }
}
