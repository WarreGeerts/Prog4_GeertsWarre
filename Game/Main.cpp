#include <SDL3/SDL_main.h>
#include "Singletons/RealTimeEditor/ComponentFactory.h"
#include "Singletons/RealTimeEditor/SceneSerializer.h"
#include "States/GSManager.h"
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Minigin.h"
#include "Singletons/SceneManager.h"
#include <filesystem>
#include "Components/GameComponents.h"
namespace fs = std::filesystem;

namespace game {
    static void load() //loads once
    {
        //game components
        ge::ComponentFactory::GetInstance().Register<ThrashCacheComponent>("ThrashCacheComponent");
        ge::ComponentFactory::GetInstance().Register<LivesComponent>("LivesComponent");
        ge::ComponentFactory::GetInstance().Register<ScoreComponent>("ScoreComponent");
        ge::ComponentFactory::GetInstance().Register<LivesDisplayComponent>("LivesDisplayComponent");
        ge::ComponentFactory::GetInstance().Register<ScoreDisplayComponent>("ScoreDisplayComponent");
        ge::ComponentFactory::GetInstance().Register<UiAutoCycleComponent>("UiAutoCycleComponent");
        ge::ComponentFactory::GetInstance().Register<GameInputComponent>("GameInputComponent");
        ge::ComponentFactory::GetInstance().Register<UiGameModeSelector>("UiGameModeSelector");
        ge::ComponentFactory::GetInstance().Register<CollisionGridComponent>("CollisionGridComponent");

        //load in all scenes at start
        ge::SceneManager::GetInstance().CreateScene("Main");
        int sceneIdx{0};
        const std::filesystem::path scenesDir = std::filesystem::current_path() / "Data" / "Scenes";

        const std::filesystem::path menuPath = scenesDir / ("Menu.json");
        ge::SceneSerializer::LoadScene(menuPath.string(), ge::SceneManager::GetInstance().GetSceneByIdx(sceneIdx));
        ++sceneIdx;

        ge::SceneManager::GetInstance().CreateScene("Lobby");
        const std::filesystem::path lobbyPath = scenesDir / ("Lobby.json");
        ge::SceneSerializer::LoadScene(lobbyPath.string(), ge::SceneManager::GetInstance().GetSceneByIdx(sceneIdx));
        ++sceneIdx;

        //load game state machine
        auto &gameStateManager = GSManager::GetInstance();
        gameStateManager.Initialize();
    }
}

    int main(int, char *[]) {
#if __EMSCRIPTEN__
        fs::path data_location = "";
#else
        fs::path data_location = "./Data/";
        if (!fs::exists(data_location))
            data_location = "../Data/";
#endif
        ge::Minigin engine(data_location);
        engine.Run(game::load);

        return 0;
    }
