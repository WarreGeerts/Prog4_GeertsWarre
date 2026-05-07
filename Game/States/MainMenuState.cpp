#include "MainMenuState.h"

#include "GameStateManager.h"
#include "LobbyState.h"
#include "Singletons/SceneManager.h"

namespace dae {
    void MainMenuState::OnEnter() {
        SceneManager::GetInstance().SelectSceneByName("Menu");

        m_Handle = EventManager::GetInstance().AttachEvent(
        EventRegistry::GO_TO_LOBBY,
        [](const Event&) {
            GameStateManager::GetInstance().ChangeState(new LobbyState());
        }
    );
    }
    void MainMenuState::OnExit() {
        EventManager::GetInstance().DetachEvent(m_Handle);
    }
}
