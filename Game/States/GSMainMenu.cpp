#include "GSMainMenu.h"
#include "GSManager.h"
#include "GSLobby.h"
#include "Singletons/SceneManager.h"

namespace game {
    void GSMainMenu::OnEnter() {
        ge::SceneManager::GetInstance().SelectSceneByName("Menu");

        HandleInput();
    }


    void GSMainMenu::OnExit() {
        //detach when leaving the scene
        ge::EventManager::GetInstance().DetachEvent(m_Handle);
    }

    void GSMainMenu::HandleInput() {
        m_Handle = ge::EventManager::GetInstance().AttachEvent(
           ge::EventRegistry::A_BUTTON_PRESSED,
           [](const ge::Event &) {
              GSManager::GetInstance().ChangeState(new GSLobby());
           });
    }
}
