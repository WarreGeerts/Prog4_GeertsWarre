#include "SceneManager.h"
#include <utility>
#include "Scene.h"

void dae::SceneManager::Update() {
    for (auto &scene: m_scenes) {
        scene->Update();
    }
}

void dae::SceneManager::Render() const {
    for (const auto &scene: m_scenes) {
        scene->Render();
    }
}

dae::Scene &dae::SceneManager::CreateScene(std::string name) {
    m_scenes.emplace_back(new Scene(std::move(name)));
    return *m_scenes.back();
}

dae::Scene &dae::SceneManager::GetSceneByName(const std::string &name) const {
    for (const auto &scene: m_scenes) {
        if (scene->GetName() == name) {
            return *scene;
        }
    }
    throw std::runtime_error("Scene '" + name + "' not found");
}

dae::Scene &dae::SceneManager::GetSceneByIdx(const int idx) const {
    if (m_scenes[idx] != nullptr) {
        return *m_scenes[idx];
    }
    throw std::runtime_error("Scene at Idx: '" + std::to_string(idx) + "' not found");
}
