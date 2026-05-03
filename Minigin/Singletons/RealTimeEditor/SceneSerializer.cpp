//
// Created by geert on 29/04/2026.
//
#include "SceneSerializer.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <SDL3/SDL_log.h>
#include "ComponentFactory.h"
#include "EditorGui.h"
#include "Input/InputManager.h"

void dae::SceneSerializer::SaveScene(const std::string &filePath, const Scene &scene) {
    //add scene to json
    nlohmann::ordered_json sceneJson;

    sceneJson["scene_id"] = scene.GetId();
    sceneJson["scene_name"] = scene.GetName();
    sceneJson["gameobjects"] = nlohmann::json::array();

    for (const auto &GO: scene.GetGameObjects()) {
        //add GO to json
        nlohmann::ordered_json goJson;
        goJson["go_id"] = GO->GetId();
        goJson["go_name"] = GO->GetName();
        goJson["enabled"] = GO->GetIsEnabled();
        goJson["transform"] = GO->GetTransform()->Serialize();
        goJson["components"] = nlohmann::json::array();

        for (const auto &comp: GO->GetComponents()) {
            //add co to json
            nlohmann::ordered_json compJson;
            compJson["comp_id"] = comp->GetId();
            compJson["comp_name"] = comp->GetName();
            compJson["active"] = comp->GetActive();
            compJson["comp_data"] = comp->Serialize();

            goJson["components"].push_back(compJson);
        }

        sceneJson["gameobjects"].push_back(goJson);
    }

    std::ofstream file(filePath);
    if (file.is_open()) {
        file << sceneJson.dump(4);
    }
}

void dae::SceneSerializer::LoadScene(const std::string &filePath, Scene &scene) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        SDL_Log("Failed to open scene file: %s", filePath.c_str());
        return; // Don't proceed if the file isn't there!
    }
    SDL_Log("Opened scene file: %s", filePath.c_str());

    nlohmann::ordered_json sceneJson;
    try {
        file >> sceneJson;
    } catch (const nlohmann::json::parse_error &e) {
        SDL_Log("JSON Parse Error: %s", e.what());
        return;
    }

    InputManager::GetInstance().ClearBindings();
    EditorGui::ClearSelection();
    Component::ClearIds();
    GameObject::ClearIds();
    scene.ClearGameObjects();

    std::unordered_map<int, GameObject *> goById;

    for (const auto &goData: sceneJson["gameobjects"]) {
        int goId = goData["go_id"];
        std::string goName = goData["go_name"];
        bool enabled = goData["enabled"];

        auto go = std::make_unique<GameObject>(goName);
        go->SetIsEnabled(enabled);

        go->GetTransform()->Deserialize(goData);

        for (const auto &compData: goData["components"]) {
            int compId = compData["comp_id"];
            std::string compName = compData["comp_name"];
            bool compEnabled = compData["active"];
            auto compDataJson = compData["comp_data"];

            auto comp = ComponentFactory::Create(compName, go.get());
            comp->ChangeActive(compEnabled);
            if (comp) {
                comp->Deserialize(compDataJson);
                go->AddComponent(std::move(comp));
            }
        }

        scene.Add(std::move(go));
    }

    SDL_Log("File loaded");
}
