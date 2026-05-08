#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Components/EngineComponents.h"
#include "Components/GameInputComponent.h"
#include "Singletons/RealTimeEditor/ComponentFactory.h"
#include "Singletons/RealTimeEditor/SceneSerializer.h"
#include "States/GSManager.h"
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Minigin.h"
#include "Singletons/SceneManager.h"
#include "Scene.h"
#include <filesystem>
#include "Components/GameComponents.h"
namespace fs = std::filesystem;
using namespace dae;

static void load() //loads once
{
    //game components
    ComponentFactory::GetInstance().Register<ThrashCacheComponent>("ThrashCacheComponent");
    ComponentFactory::GetInstance().Register<LivesComponent>("LivesComponent");
    ComponentFactory::GetInstance().Register<ScoreComponent>("ScoreComponent");
    ComponentFactory::GetInstance().Register<LivesDisplayComponent>("LivesDisplayComponent");
    ComponentFactory::GetInstance().Register<ScoreDisplayComponent>("ScoreDisplayComponent");
    ComponentFactory::GetInstance().Register<UiAutoCycleComponent>("UiAutoCycleComponent");
    ComponentFactory::GetInstance().Register<GameInputComponent>("GameInputComponent");

    //load in all scenes at start
    SceneManager::GetInstance().CreateScene("Main");
    int sceneIdx {0};
    const std::filesystem::path scenesDir = std::filesystem::current_path() / "Data" / "Scenes";

    const std::filesystem::path menuPath = scenesDir / ("Menu.json");
    SceneSerializer::LoadScene(menuPath.string(), SceneManager::GetInstance().GetSceneByIdx(sceneIdx));
    ++sceneIdx;

    SceneManager::GetInstance().CreateScene("Lobby");
    const std::filesystem::path lobbyPath = scenesDir / ("Lobby.json");
    SceneSerializer::LoadScene(lobbyPath.string(), SceneManager::GetInstance().GetSceneByIdx(sceneIdx));
    ++sceneIdx;


    //load game state machine
    auto& gameStateManager = GSManager::GetInstance();
    gameStateManager.Initialize();
}

int main(int, char *[]) {


#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    Minigin engine(data_location);
    engine.Run(load);

    return 0;
}
