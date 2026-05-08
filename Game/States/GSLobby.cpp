#include "GSLobby.h"
#include "Singletons/SceneManager.h"

namespace game {
    void GSLobby::OnEnter() {
        ge::SceneManager::GetInstance().SelectSceneByName("Lobby");
    }
}
