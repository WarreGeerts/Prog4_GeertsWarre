#include "GMSCoop.h"

#include "Singletons/SceneManager.h"

void GMSCoop::OnEnter() {
    ge::SceneManager::GetInstance().SelectSceneByName("Game_Coop");
}
void GMSCoop::OnExit() {}
