#include "GSManager.h"
#include "GSMainMenu.h"

namespace game {
    void GSManager::Initialize() {
        ChangeState(new GSMainMenu);
    }

    void GSManager::ChangeState(GameState *newState) {
        if (m_CurrentGameState) {
            m_CurrentGameState->OnExit();
            delete m_CurrentGameState;
        }

        m_CurrentGameState = newState;
        m_CurrentGameState->OnEnter();
    }

    void GSManager::Update() {
        if (m_CurrentGameState) m_CurrentGameState->Update();
    }
}
