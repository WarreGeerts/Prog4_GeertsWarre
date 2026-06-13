#include <SDL3/SDL_main.h>

#include "EngineUpdater.h"
#include "Events.h"
#include "GameUpdater.h"
#include "Commands/MuteCommand.h"
#include "Commands/SkipLevelCommand.h"
#include "Input/InputManager.h"
#include "SDL3/SDL_log.h"
#include "Singletons/RealTimeEditor/ComponentFactory.h"
#include "Singletons/RealTimeEditor/SceneSerializer.h"
#include "Sound/ServiceLocator.h"
#include "States/GMSManager.h"
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
        ge::ComponentFactory::GetInstance().Register<PlayerCollisionComponent>("PlayerCollisionComponent");
        ge::ComponentFactory::GetInstance().Register<HamburgerMovementComponent>("HamburgerMovementComponent");
        ge::ComponentFactory::GetInstance().Register<HamburgerHolderComponent>("HamburgerHolderComponent");
        ge::ComponentFactory::GetInstance().Register<SpawnerComponent>("SpawnerComponent");
        ge::ComponentFactory::GetInstance().Register<AIControllerComponent>("AIControllerComponent");
        ge::ComponentFactory::GetInstance().Register<DamageComponent>("DamageComponent");
        ge::ComponentFactory::GetInstance().Register<EnemyComponent>("EnemyComponent");
        ge::ComponentFactory::GetInstance().Register<EnemyComponent>("EnemyComponent");
        ge::ComponentFactory::GetInstance().Register<HamburgerManager>("HamburgerManager");

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

        ge::SceneManager::GetInstance().CreateScene("Level_1");
        const std::filesystem::path L1Path = scenesDir / ("Level_1.json");
        ge::SceneSerializer::LoadScene(L1Path.string(), ge::SceneManager::GetInstance().GetSceneByIdx(sceneIdx));
        ++sceneIdx;

        ge::SceneManager::GetInstance().CreateScene("GameOver");
        const std::filesystem::path GameOverPath = scenesDir / ("GameOver.json");
        ge::SceneSerializer::LoadScene(GameOverPath.string(), ge::SceneManager::GetInstance().GetSceneByIdx(sceneIdx));
        ++sceneIdx;

        //events
        EventRegistry::Initialize();

        //load game state machine
        auto &gameStateManager = GSManager::GetInstance();
        gameStateManager.Initialize();

        auto &gameModeStateManager = GMSManager::GetInstance();
        gameModeStateManager.Initialize();

        //Debug commands
        auto &Input{ge::InputManager::GetInstance()};
        Input.AddBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_F1), true},
                             std::make_unique<SkipLevelCommand>());
        Input.AddBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_F2), true},
                             std::make_unique<MuteCommand>());

        //Sounds
        auto &ss = ge::ServiceLocator::GetSoundSystem();

        ss.Load(0, "BGM.wav");
        ss.Load(1, "Game Start.wav");
        ss.Load(2, "Round Clear.wav");
        ss.Load(3, "Death.wav");
        ss.Load(4, "Burger Step.wav");
        ss.Load(5, "Burger Fall.wav");
        ss.Load(6, "Burger Land.wav");
        ss.Load(7, "Enemy Squahed.wav");
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
    auto gameUpdater = std::make_unique<GameUpdater>();
    ge::EngineUpdater::SetUpdater(gameUpdater.get());
    engine.Run(game::load);

    return 0;
}
