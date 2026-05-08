#include "GSLobby.h"
#include "Singletons/SceneManager.h"

namespace dae {
    void GSLobby::OnEnter() {
        SceneManager::GetInstance().SelectSceneByName("Lobby");
    }
}
