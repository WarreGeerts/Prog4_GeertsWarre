#include "GMSSinglePlayer.h"
#include "Singletons/SceneManager.h"

namespace game {
    void GMSSinglePlayer::OnEnter() {
        ge::SceneManager::GetInstance().SelectSceneByName("Level_1");
    }

    void GMSSinglePlayer::OnExit() {}
}
