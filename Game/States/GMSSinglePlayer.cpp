#include "GMSSinglePlayer.h"

#include "Singletons/SceneManager.h"

void game::GMSSinglePlayer::OnEnter() {
    ge::SceneManager::GetInstance().SelectSceneByName("Level_1");
}
void game::GMSSinglePlayer::OnExit() {}
