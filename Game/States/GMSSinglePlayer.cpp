#include "GMSSinglePlayer.h"

#include "Singletons/SceneManager.h"

void game::GMSSinglePlayer::OnEnter() {
    ge::SceneManager::GetInstance().SelectSceneByName("Game_SinglePlayer");
}
void game::GMSSinglePlayer::OnExit() {}
