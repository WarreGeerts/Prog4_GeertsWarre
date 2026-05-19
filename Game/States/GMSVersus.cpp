#include "GMSVersus.h"
#include "Singletons/SceneManager.h"

namespace game {
    void GMSVersus::OnEnter() {
        ge::SceneManager::GetInstance().SelectSceneByName("Level_1");
    }

    void GMSVersus::OnExit() {}
}
