#include <algorithm>
#include "Scene.h"
#include "Component.h"
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

void Scene::Update() {
    for (auto it = m_objects.begin(); it != m_objects.end();) {
        {
            if ((*it)->MarkedForDeletion()) {
                it = m_objects.erase(it);
            } else {
                (*it)->Update();
                ++it;
            }
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
