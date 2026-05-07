#include "GameStateManager.h"

namespace dae {
    void GameStateManager::ChangeState(GameState *newState) {
        if (m_CurrentGameState) {
            m_CurrentGameState->OnExit();
            delete m_CurrentGameState;
        }

        m_CurrentGameState = newState;
        m_CurrentGameState->OnEnter();
    }

    void GameStateManager::Update() {
        if (m_CurrentGameState) m_CurrentGameState->Update();
    }

    void GameStateManager::Render() const {
        if (m_CurrentGameState) m_CurrentGameState->Render();
    }
}
