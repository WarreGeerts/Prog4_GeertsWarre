#include "GSManager.h"
#include "GSMainMenu.h"

namespace game {
    void GSManager::Initialize() {
        ChangeState(std::make_unique<GSMainMenu>());
    }

    void GSManager::ChangeState(std::unique_ptr<GameState> newState) {
        if (m_CurrentGameState) {
            m_CurrentGameState->OnExit();
        }

        m_CurrentGameState = std::move(newState);
        m_CurrentGameState->OnEnter();
    }

    void GSManager::Update() {
        if (m_CurrentGameState) m_CurrentGameState->Update();
    }
}
