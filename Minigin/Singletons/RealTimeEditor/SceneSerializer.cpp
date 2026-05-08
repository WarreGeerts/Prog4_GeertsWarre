#include "SceneSerializer.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <SDL3/SDL_log.h>
#include "ComponentFactory.h"
#include "EditorGui.h"
#include "Input/InputManager.h"
#include "Singletons/SceneManager.h"

namespace ge {
    void SceneSerializer::SaveScene(const std::string &filePath, const Scene &scene) {
        std::filesystem::path path{filePath};
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }

        //add scene to json
        nlohmann::ordered_json sceneJson;

        sceneJson["scene_id"] = scene.GetId();
        sceneJson["scene_name"] = scene.GetName();
        sceneJson["gameobjects"] = nlohmann::json::array();

        for (const auto &GO: scene.GetGameObjects()) {
            //add GO to json
            nlohmann::ordered_json goJson;
            goJson["go_id"] = GO->GetId();
            if (GO->GetParent()) {
                goJson["parent_id"] = GO->GetParent()->GetId();
            } else {
                goJson["parent_id"] = -1;
            }
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

        std::ofstream file(filePath, std::ios::out | std::ios::trunc);

        if (file.is_open()) {
            file << sceneJson.dump(4);
            file.close();
            SDL_Log("Scene successfully saved/overwritten: %s", filePath.c_str());
        } else {
            SDL_Log("CRITICAL: Failed to overwrite file at %s", filePath.c_str());
        }
    }

    void SceneSerializer::LoadScene(const std::string &filePath, Scene& scene) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            SDL_Log("Failed to open scene file: %s", filePath.c_str());
            return;
        }
        SDL_Log("Opened scene file: %s", filePath.c_str());

        nlohmann::ordered_json sceneJson;
        try {
            file >> sceneJson;
        } catch (const nlohmann::json::parse_error &e) {
            SDL_Log("JSON Parse Error: %s", e.what());
            return;
        }

        //InputManager::GetInstance().ClearBindings();
        EditorGui::ClearSelection();
        //Component::ClearIds();
        //GameObject::ClearIds();
        scene.ClearGameObjects();

        std::unordered_map<int, GameObject *> goMap;
        std::vector<std::pair<GameObject *, int> > parentRequests;

        if (sceneJson.contains("scene_name")) {
            scene.SetName(sceneJson["scene_name"]);
        }

        for (const auto &goData: sceneJson["gameobjects"]) {
            int goId = goData["go_id"];
            std::string goName = goData["go_name"];
            bool enabled = goData["enabled"];

            auto go = std::make_unique<GameObject>(goName);
            GameObject *goPtr = go.get();
            go->SetActive(enabled);

            go->GetTransform()->Deserialize(goData);

            for (const auto &compData: goData["components"]) {
                //int compId = compData["comp_id"];
                std::string compName = compData["comp_name"];
                bool compEnabled = compData["active"];
                const auto& compDataJson = compData["comp_data"];

                auto comp = ComponentFactory::GetInstance().Create(compName, go.get());
                comp->ChangeActive(compEnabled);
                if (comp) {
                    comp->ChangeActive(compEnabled);
                    comp->Deserialize(compDataJson);
                    go->AddComponent(std::move(comp));
                }
            }

            goMap[goId] = goPtr;

            if (goData.contains("parent_id")) {
                int pId = goData["parent_id"];
                if (pId != -1) {
                    parentRequests.emplace_back(goPtr, pId);
                }
            }

            scene.Add(std::move(go));
        }

        for (auto &[GO, ID]: parentRequests) {
            GameObject *child = GO;
            int parentId = ID;

            if (goMap.contains(parentId)) {
                child->SetParent(goMap[parentId], true);
            }
        }

        SDL_Log("Scene '%s' loaded successfully.", scene.GetName().c_str());
    }
}
