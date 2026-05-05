#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Components/EngineComponents.h"
#include "Singletons/RealTimeEditor/ComponentFactory.h"
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
    //engine
    ComponentFactory::Register<TransformComponent>("TransformComponent");
    ComponentFactory::Register<RenderComponent>("RenderComponent");
    ComponentFactory::Register<TextComponent>("TextComponent");
    ComponentFactory::Register<SpriteComponent>("SpriteComponent");
    ComponentFactory::Register<FPSComponent>("FPSComponent");
    ComponentFactory::Register<CharacterControllerComponent>("CharacterControllerComponent");
    ComponentFactory::Register<RotateComponent>("RotateComponent");
    //game
    ComponentFactory::Register<ThrashCacheComponent>("ThrashCacheComponent");
    ComponentFactory::Register<LivesComponent>("LivesComponent");
    ComponentFactory::Register<ScoreComponent>("ScoreComponent");
    ComponentFactory::Register<LivesDisplayComponent>("LivesDisplayComponent");
    ComponentFactory::Register<ScoreDisplayComponent>("ScoreDisplayComponent");


    auto &scene = SceneManager::GetInstance().CreateScene("Main");
    auto go = std::make_unique<GameObject>("Background");
    go->SetLocalPosition({0, 0, 0});
    go->AddComponent(std::make_unique<RenderComponent>(go.get(), "background.png"));
    scene.Add(std::move(go));
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
