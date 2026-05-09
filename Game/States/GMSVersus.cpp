#include "GMSVersus.h"

#include "Singletons/SceneManager.h"

void GMSVersus::OnEnter() {
    ge::SceneManager::GetInstance().SelectSceneByName("Game_Versus");
}
void GMSVersus::OnExit() {}
