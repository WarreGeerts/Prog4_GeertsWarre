#include "SceneManager.h"
#include <utility>
#include "Scene.h"
#include <stdexcept>

void dae::SceneManager::Update() {
    m_scenes.at(m_SceneIdx)->Update();

    /*for (auto &scene: m_scenes) {
        scene->Update();
    }*/
}

void dae::SceneManager::Render() const {
    m_scenes.at(m_SceneIdx)->Render();

    /*for (const auto &scene: m_scenes) {
        scene->Render();
    }*/
}

void dae::SceneManager::RenderGUI() const {
    m_scenes.at(m_SceneIdx)->RenderGUI();

    /*for (const auto &scene: m_scenes) {
        scene->RenderGUI();
    }*/
}

int dae::SceneManager::GetCurrentSceneIdx() const { return m_SceneIdx; }

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

std::vector<int> dae::SceneManager::GetSceneSizes() const {
    std::vector<int> sizes;
    for (const auto &scene: m_scenes) {
        sizes.push_back(scene->GetSceneSize());
    }
    return sizes;
}

const std::vector<std::unique_ptr<dae::Scene> > &dae::SceneManager::GetScenes() const {
    return m_scenes;
}

void dae::SceneManager::SelectSceneByIndex(const int idx) {
    if (idx < m_scenes.size()) {
        m_SceneIdx = idx;
    } else {
        throw std::runtime_error("Idx: '" + std::to_string(idx) + "' is Invalid");
    }
}

void dae::SceneManager::SelectSceneByName(const std::string &name) {
    for (int idx{0}; idx < m_scenes.size(); ++idx) {
        if (m_scenes[idx]->GetName() == name) {
            m_SceneIdx = idx;
            return;
        }
    }
    throw std::runtime_error("Name: '" + name + "' is Invalid");
}
