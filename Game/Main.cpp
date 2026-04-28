#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../Minigin/Components/CharacterControllerComponent.h"
#include "../Minigin/Input/InputManager.h"
#include "../Minigin/Components/Components.h"
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "../Minigin/Minigin.h"
#include "../Minigin/Singletons/SceneManager.h"
#include "../Minigin/Singletons/ResourceManager.h"
#include "../Minigin/Scene.h"
#include <filesystem>
namespace fs = std::filesystem;
using namespace dae;

static void load() //loads once
{
    auto &scene = SceneManager::GetInstance().CreateScene("Main");
    const auto fontLinguaTitle = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    const auto fontLinguaLower = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
    constexpr SpriteFraming spriteFraming{15, 11, 16.f, 16.f};

    //Background ****
    //GameObject ====
    auto go = std::make_unique<GameObject>("Background");
    go->SetLocalPosition({0, 0, 0});
    //Add components =====
    go->AddComponent(std::make_unique<RenderComponent>(go.get(), "background.png"));
    //add to scene ====
    scene.Add(std::move(go));

    //Logo ****
    //GameObject ====
    go = std::make_unique<GameObject>("Logo");
    go->SetLocalPosition({358, 180, 0});
    //Add components =====
    go->AddComponent(std::make_unique<RenderComponent>(go.get(), "logo.png"));
    //add to scene ====
    go->SetParent(&scene.GetGameObjectByName("Background"), true);
    scene.Add(std::move(go));

    //Title text ****
    //GameObject ====
    go = std::make_unique<GameObject>("TitleText");
    go->SetLocalPosition({292, 20, 0});
    //Add components =====
    go->AddComponent(std::make_unique<TextComponent>(go.get(), "Programming 4 Assignment", "Lingua.otf", 36));
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    //Initialise components ====
    //add to scene ====
    scene.Add(std::move(go));

    //Keybinds PC Text ****
    //GameObject ====
    go = std::make_unique<GameObject>("KeybindsPCText");
    go->SetLocalPosition({10, 100, 0});
    //Add components =====
    go->AddComponent(std::make_unique<TextComponent>(go.get(),
                                                     "Use WASD to move Burger Man, C to inflict damage, Z and X to drop down the burgers",
                                                     "Lingua.otf", 18));
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    //Initialise components ====
    //add to scene ====
    scene.Add(std::move(go));

    //Keybinds Controller Text ****
    //GameObject ====
    go = std::make_unique<GameObject>("KeybindsControllerText");
    go->SetLocalPosition({10, 120, 0});
    //Add components =====
    go->AddComponent(std::make_unique<TextComponent>(go.get(),
                                                     "Use D-Pad to move The Bean, X to inflict damage, A and B to drop down the burgers",
                                                     "Lingua.otf", 18));
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    //Initialise components ====
    //add to scene ====
    scene.Add(std::move(go));

    //FPS counter ****
    //GameObject ====
    go = std::make_unique<GameObject>("FPSCounter");
    go->SetLocalPosition({10, 10, 0});
    //Add components =====
    go->AddComponent(std::make_unique<FPSComponent>(go.get()));
    go->AddComponent(std::make_unique<TextComponent>(go.get(), " ", "Lingua.otf", 36));
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    //Initialise components ====
    //FPSComponent
    go->GetComponent<FPSComponent>()->SetRefTextComponent(go->GetComponent<TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //BurgerMan Player ****
    //GameObject ====
    go = std::make_unique<GameObject>("BurgerMan");
    go->SetLocalPosition({200, 200, 0});
    //Add components ====
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<CharacterControllerComponent>(go.get(), 60.f, true));
    go->AddComponent(std::make_unique<SpriteComponent>(go.get(), "Sprites.png", spriteFraming, 1));
    go->AddComponent(
        std::make_unique<LivesComponent>(go.get(), std::vector{EventRegistry::P1_DMG}, EventRegistry::P1_HEALTH_UPDATE,
                                         3));
    go->AddComponent(
        std::make_unique<ScoreComponent>(
            go.get(), std::vector{EventRegistry::P1_BURGER_FALL, EventRegistry::P1_ENEMY_KILL},
            EventRegistry::P1_SCORE_UPDATE));
    //Initialise components ====
    //SpriteComponent
    go->GetComponent<SpriteComponent>()->SetScale(3);
    //add to scene ====
    scene.Add(std::move(go));

    //BurgerMan Lives text ****
    //GameObject ====
    go = std::make_unique<GameObject>("BurgerManLivesText");
    go->SetLocalPosition({10, 160, 0});
    //Add components =====
    go->AddComponent(std::make_unique<LivesDisplayComponent>(go.get(), EventRegistry::P1_HEALTH_UPDATE));
    go->AddComponent(std::make_unique<TextComponent>(go.get(), " ", "Lingua.otf", 18));
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    //Initialise components ====
    //LivesDisplayComponent
    go->GetComponent<LivesDisplayComponent>()->SetRefTextComponent(go->GetComponent<TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //BurgerMan Score text ****
    //GameObject ====
    go = std::make_unique<GameObject>("BurgerManScoreText");
    go->SetLocalPosition({10, 180, 0});
    //Add components =====
    go->AddComponent(std::make_unique<ScoreDisplayComponent>(go.get(), EventRegistry::P1_SCORE_UPDATE));
    go->AddComponent(std::make_unique<TextComponent>(go.get(), " ", "Lingua.otf", 18));
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    //Initialise components ====
    //LivesDisplayComponent
    go->GetComponent<ScoreDisplayComponent>()->SetRefTextComponent(go->GetComponent<TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //Bean Player ****
    //GameObject ====
    go = std::make_unique<GameObject>("Bean");
    go->SetLocalPosition({200, 250, 0});
    //Add components ====
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<CharacterControllerComponent>(go.get(), 120.f, false));
    go->AddComponent(std::make_unique<SpriteComponent>(go.get(), "Sprites.png", spriteFraming, 30));
    go->AddComponent(
        std::make_unique<LivesComponent>(go.get(), std::vector{EventRegistry::P2_DMG}, EventRegistry::P2_HEALTH_UPDATE,
                                         3));
    go->AddComponent(
        std::make_unique<ScoreComponent>(
            go.get(), std::vector{EventRegistry::P2_BURGER_FALL, EventRegistry::P2_ENEMY_KILL},
            EventRegistry::P2_SCORE_UPDATE));
    //Initialise components ====
    //SpriteComponent
    go->GetComponent<SpriteComponent>()->SetScale(3);
    //add to scene ====
    scene.Add(std::move(go));

    //Bean Lives text ****
    //GameObject ====
    go = std::make_unique<GameObject>("BeanLivesText");
    go->SetLocalPosition({10, 200, 0});
    //Add components =====
    go->AddComponent(std::make_unique<LivesDisplayComponent>(go.get(), EventRegistry::P2_HEALTH_UPDATE));
    go->AddComponent(std::make_unique<TextComponent>(go.get(), " ", "Lingua.otf", 18));
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    //Initialise components ====
    //LivesDisplayComponent
    go->GetComponent<LivesDisplayComponent>()->SetRefTextComponent(go->GetComponent<TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //Bean Score text ****
    //GameObject ====
    go = std::make_unique<GameObject>("BeanScoreText");
    go->SetLocalPosition({10, 220, 0});
    //Add components =====
    go->AddComponent(std::make_unique<ScoreDisplayComponent>(go.get(), EventRegistry::P2_SCORE_UPDATE));
    go->AddComponent(std::make_unique<TextComponent>(go.get(), " ", "Lingua.otf", 18));
    go->AddComponent(std::make_unique<RenderComponent>(go.get()));
    //Initialise components ====
    //LivesDisplayComponent
    go->GetComponent<ScoreDisplayComponent>()->SetRefTextComponent(go->GetComponent<TextComponent>());
    //add to scene ====
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
