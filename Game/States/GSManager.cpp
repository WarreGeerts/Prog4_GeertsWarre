#include "GSManager.h"
#include "GSGame.h"
#include "GSGameOver.h"
#include "GSMainMenu.h"
#include "../../cmake-build-debug/_deps/sdl3-src/include/SDL3/SDL_log.h"

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

    void GSManager::NextLevel() {
        SDL_Log("next level . . .");
        if (dynamic_cast<GSGame *>(m_CurrentGameState.get()) != nullptr) {
            m_CurrentGameState->NextLevel();
        }
    }

    void GSManager::GameOver() {
        ChangeState(std::make_unique<GSGameOver>());
    }
}
