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
    auto fontLingua = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    //GameObject ====
    auto go = std::make_unique<dae::GameObject>("Background");
    //Add components =====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //RenderComponent
    go->GetComponent<dae::RenderComponent>()->SetTexture("background.png");
    //transformComponent
    go->GetComponent<dae::TransformComponent>()->SetPosition(0, 0);
    //add to scene ====
    scene.Add(std::move(go));

    //GameObject ====
    go = std::make_unique<dae::GameObject>("Logo");
    //Add components =====
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //RenderComponent
    go->GetComponent<dae::RenderComponent>()->SetTexture("logo.png");
    //transformComponent
    go->GetComponent<dae::TransformComponent>()->SetPosition(358, 180);
    //add to scene ====
    scene.Add(std::move(go));

    //GameObject ====
    go = std::make_unique<dae::GameObject>("TitleText");
    //Add components =====
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetText("Programming 4 Assignment");
    go->GetComponent<dae::TextComponent>()->SetFont(fontLingua);
    go->GetComponent<dae::TextComponent>()->SetColor({255, 255, 255, 255});
    go->GetComponent<dae::TextComponent>()->GetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //TransformComponent
    go->GetComponent<dae::TransformComponent>()->SetPosition(292, 20);
    //add to scene ====
    scene.Add(std::move(go));

    //GameObject ====
    go = std::make_unique<dae::GameObject>("FPSCounter");
    //Add components =====
    go->AddComponent(std::make_unique<dae::FPSComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TextComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::RenderComponent>(go.get()));
    go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
    //Initialise components ====
    //TextComponent
    go->GetComponent<dae::TextComponent>()->SetText(std::to_string(go->GetComponent<dae::FPSComponent>()->GetFPS()));
    go->GetComponent<dae::TextComponent>()->SetFont(fontLingua);
    go->GetComponent<dae::TextComponent>()->SetColor({255, 255, 255, 255});
    go->GetComponent<dae::TextComponent>()->GetRefRenderComponent(go->GetComponent<dae::RenderComponent>());
    //FPSComponent
    go->GetComponent<dae::FPSComponent>()->GetRefTextComponent(go->GetComponent<dae::TextComponent>());
    //transformComponent
    go->GetComponent<dae::TransformComponent>()->SetPosition(0, 0);
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
