#include "GSMainMenu.h"
#include "GSManager.h"
#include "GSLobby.h"
#include "Singletons/SceneManager.h"

namespace dae {
    void GSMainMenu::OnEnter() {
        SceneManager::GetInstance().SelectSceneByName("Menu");

        HandleInput();
    }


    void GSMainMenu::OnExit() {
        //detach when leaving the scene
        EventManager::GetInstance().DetachEvent(m_Handle);
    }

    void GSMainMenu::HandleInput() {
        m_Handle = EventManager::GetInstance().AttachEvent(
           EventRegistry::GO_TO_LOBBY,
           [](const Event &) {
              GSManager::GetInstance().ChangeState(new GSLobby());
           });
    }
}
