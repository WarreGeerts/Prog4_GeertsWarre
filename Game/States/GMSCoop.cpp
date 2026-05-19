#include "GMSCoop.h"
#include "Singletons/SceneManager.h"

namespace game {
    void GMSCoop::OnEnter() {
        ge::SceneManager::GetInstance().SelectSceneByName("Level_1");
    }

    void GMSCoop::OnExit() {}
}
