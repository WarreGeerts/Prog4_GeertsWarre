#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Components.h"
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include <filesystem>
namespace fs = std::filesystem;

static void load() //loads once
{
    auto &scene = dae::SceneManager::GetInstance().CreateScene("Main");
    const auto fontLingua = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    //Background ****
    //GameObject ====
    auto go = std::make_unique<dae::GameObject>("Background");
    go->SetLocalPosition({0, 0, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //RenderComponent
    go->GetComponent<dae::RenderComponent>()->SetTexture("background.png");
    //add to scene ====
    scene.Add(std::move(go));

    //Logo ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("Logo");
    go->SetLocalPosition({358, 180, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //RenderComponent
    go->GetComponent<dae::RenderComponent>()->SetTexture("logo.png");
    //add to scene ====
    scene.Add(std::move(go));

    //Title text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("TitleText");
    go->SetLocalPosition({292, 20, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    dae::TextComponent &tc1{*go->GetComponent<dae::TextComponent>()};
    tc1.SetText("Programming 4 Assignment");
    tc1.SetFont(fontLingua);
    tc1.SetColor({255, 255, 255, 255});
    tc1.SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //Title text ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("InfoTextF1");
    go->SetLocalPosition({0, 540, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    dae::TextComponent &tc2{*go->GetComponent<dae::TextComponent>()};
    tc2.SetText("F1 to toggle ImGui");
    tc2.SetFont(fontLingua);
    tc2.SetColor({255, 255, 0, 255});
    tc2.SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //FPS counter ****
    //GameObject ====
    go = std::make_unique<dae::GameObject>("FPSCounter");
    go->SetLocalPosition({0, 0, 0});
    //Add components =====
    go->AddComponent(std::make_unique<dae::FPSComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    dae::TextComponent &tc3{*go->GetComponent<dae::TextComponent>()};
    tc3.SetFont(fontLingua);
    tc3.SetColor({255, 255, 255, 255});
    tc3.SetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //FPSComponent
    go->GetComponent<dae::FPSComponent>()->SetRefTextComponent(go->GetComponent<dae::TextComponent>());
    //add to scene ====
    scene.Add(std::move(go));

    //Rotating parent-child ****
    //Empty-parent
    go = std::make_unique<dae::GameObject>("BurgerManEmpty");
    go->SetLocalPosition({200, 200, 0});
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    scene.Add(std::move(go));

    //parent ----
    //GameObject ====
    go = std::make_unique<dae::GameObject>("BurgerMan");
    go->SetLocalPosition({0, 0, 0});
    //Add components ====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::RotateComponent>(go.get()));
    //Initialise components ====
    //RenderComponent
    go->GetComponent<dae::RenderComponent>()->SetTexture("BurgerMan.png");
    //RotateComponent
    dae::RotateComponent &rc1{*go->GetComponent<dae::RotateComponent>()};
    rc1.SetRadius(20);
    rc1.SetAngularSpeed(-1);
    //add to scene ====
    go->SetParent(&scene.GetGameObjectByName("BurgerManEmpty"), false);

    scene.Add(std::move(go));

    //child ----
    //GameObject ====
    go = std::make_unique<dae::GameObject>("Bean");
    go->SetLocalPosition({0, 0, 0});
    //Add components ====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::RotateComponent>(go.get()));
    //Initialise components ====
    //RenderComponent
    go->GetComponent<dae::RenderComponent>()->SetTexture("Bean.png");
    //RotateComponent
    dae::RotateComponent &rc2{*go->GetComponent<dae::RotateComponent>()};
    rc2.SetRadius(40);
    rc2.SetAngularSpeed(2);
    //add to scene ====
    go->SetParent(&scene.GetGameObjectByName("BurgerMan"), false);

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
