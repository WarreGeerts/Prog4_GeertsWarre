#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Components/CharacterControllerComponent.h"
#include "Input/InputManager.h"
#include "Components/Components.h"
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Minigin.h"
#include "Singletons/SceneManager.h"
#include "Singletons/ResourceManager.h"
#include "Scene.h"
#include <filesystem>
namespace fs = std::filesystem;

static void load() //loads once
{
    auto &scene = dae::SceneManager::GetInstance().CreateScene("Main");
    const auto fontLinguaTitle = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    const auto fontLinguaLower = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
    constexpr dae::SpriteFraming spriteFraming{15, 11, 16.f, 16.f};

    //Background ****
    //GameObject ====
    auto go = std::make_unique<dae::GameObject>("Background");
    go->SetLocalPosition({0, 0, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "background.png"));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //add to scene ====
    scene.Add(std::move(go));

    //Logo ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("Logo");
    go->SetLocalPosition({358, 180, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "logo.png"));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //add to scene ====
    scene.Add(std::move(go));

    //Title text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("TitleText");
    go->SetLocalPosition({292, 20, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Programming 4 Assignment", fontLinguaTitle));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //Keybinds PC Text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("KeybindsPCText");
    go->SetLocalPosition({10, 100, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get(),
                                                          "Use WASD to move Burger Man, C to inflict damage, Z and X to drop down the burgers",
                                                          fontLinguaLower));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //Keybinds Controller Text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("KeybindsControllerText");
    go->SetLocalPosition({10, 120, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get(),
                                                          "Use D-Pad to move The Bean, X to inflict damage, A and B to drop down the burgers",
                                                          fontLinguaLower));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //FPS counter ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("FPSCounter");
    go->SetLocalPosition({10, 10, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::FPSComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), " ", fontLinguaTitle));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //FPSComponent
    go->GetComponent<dae::FPSComponent>()->SetRefTextComponent(go->GetComponent<dae::TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //BurgerMan Player ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("BurgerMan");
    go->SetLocalPosition({200, 200, 0});
    //Add components ====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::CharacterControllerComponent>(go.get(), 60.f, true));
    go->AddComponent(std::make_unique<dae::SpriteComponent>(go.get(), "Sprites.png", spriteFraming, 1));
    go->AddComponent(std::make_unique<dae::LivesComponent>(go.get(), dae::P1_DMG, dae::P1_HEALTH_UPDATE, 3));
    go->AddComponent(
        std::make_unique<dae::ScoreComponent>(go.get(), std::vector{dae::P1_BURGER_FALL, dae::P1_ENEMY_KILL},
                                              dae::P1_SCORE_UPDATE));
    //Initialise components ====
    //SpriteComponent
    go->GetComponent<dae::SpriteComponent>()->SetScale(3);
    //add to scene ====
    scene.Add(std::move(go));

    //BurgerMan Lives text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("BurgerManLivesText");
    go->SetLocalPosition({10, 160, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::LivesDisplayComponent>(go.get(), dae::P1_HEALTH_UPDATE));
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), " ", fontLinguaLower));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //LivesDisplayComponent
    go->GetComponent<dae::LivesDisplayComponent>()->SetRefTextComponent(go->GetComponent<dae::TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //BurgerMan Score text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("BurgerManScoreText");
    go->SetLocalPosition({10, 180, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::ScoreDisplayComponent>(go.get(), dae::P1_SCORE_UPDATE));
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), " ", fontLinguaLower));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //LivesDisplayComponent
    go->GetComponent<dae::ScoreDisplayComponent>()->SetRefTextComponent(go->GetComponent<dae::TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //Bean Player ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("Bean");
    go->SetLocalPosition({200, 250, 0});
    //Add components ====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::CharacterControllerComponent>(go.get(), 120.f, false));
    go->AddComponent(std::make_unique<dae::SpriteComponent>(go.get(), "Sprites.png", spriteFraming, 30));
    go->AddComponent(std::make_unique<dae::LivesComponent>(go.get(), dae::P2_DMG, dae::P2_HEALTH_UPDATE, 3));
    go->AddComponent(
        std::make_unique<dae::ScoreComponent>(go.get(), std::vector{dae::P2_BURGER_FALL, dae::P2_ENEMY_KILL},
                                              dae::P2_SCORE_UPDATE));
    //Initialise components ====
    //SpriteComponent
    go->GetComponent<dae::SpriteComponent>()->SetScale(3);
    //add to scene ====
    scene.Add(std::move(go));

    //Bean Lives text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("BeanLivesText");
    go->SetLocalPosition({10, 200, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::LivesDisplayComponent>(go.get(), dae::P2_HEALTH_UPDATE));
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), " ", fontLinguaLower));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //LivesDisplayComponent
    go->GetComponent<dae::LivesDisplayComponent>()->SetRefTextComponent(go->GetComponent<dae::TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //Bean Score text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("BeanScoreText");
    go->SetLocalPosition({10, 220, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::ScoreDisplayComponent>(go.get(), dae::P2_SCORE_UPDATE));
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), " ", fontLinguaLower));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //LivesDisplayComponent
    go->GetComponent<dae::ScoreDisplayComponent>()->SetRefTextComponent(go->GetComponent<dae::TextComponent>());
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
    dae::Minigin engine(data_location);
    engine.Run(load);
    return 0;
}
