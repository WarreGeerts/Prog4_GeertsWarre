#include <algorithm>
#include "Scene.h"
#include <cassert>
#include <utility>
#include "Component.h"
#include <stdexcept>

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object) {
    assert(object != nullptr && "Cannot add a null GameObject to the scene.");
    m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject &object) {
    m_objects.erase(
        std::remove_if(
            m_objects.begin(),
            m_objects.end(),
            [&object](const auto &ptr) { return ptr.get() == &object; }
        ),
        m_objects.end()
    );
}

void Scene::RemoveAll() {
    m_objects.clear();
}

GameObject &Scene::GetGameObjectByName(const std::string &name) const {
    for (const auto &object: m_objects) {
        if (object->GetName() == name) {
            return *object;
        }
    }
    throw std::runtime_error("GameObject '" + name + "' not found");
}

GameObject &Scene::GetGameObjectByIndex(const int idx) const {
    if (m_objects[idx] != nullptr) {
        return *m_objects[idx];
    }
    throw std::runtime_error("GameObject at idx: '" + std::to_string(idx) + "' not found");
}

void Scene::Update() {
    //update everything first
    for (auto &object: m_objects) {
        object->Update();
    }
    //after update look through all objects to delete those who are marked
    for (auto &object: m_objects) {
        if (object->MarkedForDeletion()) {
           m_objects.erase(remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
        }
    }
}

void Scene::Render() const {
    for (const auto &object: m_objects) {
        for (const auto &component: object->GetComponents()) {
            component->Render();
        }
        //object->Render();
    }
}

