#include "GSGameOver.h"

#include "Singletons/SceneManager.h"

void game::GSGameOver::OnEnter() {
    ge::SceneManager::GetInstance().SelectSceneByIndex(m_GameOverSceneIndex);
}
void game::GSGameOver::OnExit() {
    //Save score to file
}
