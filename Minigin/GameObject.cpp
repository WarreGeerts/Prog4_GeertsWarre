#include "GameObject.h"
#include <algorithm>
#include <cassert>
#include <utility>
#include "Component.h"
using namespace dae;
GameObject::GameObject(std::string name) : m_Name(std::move(name)) {}
GameObject::~GameObject() = default;

void GameObject::Update() {
    for (auto &component: m_components) {
        component->Update();
    }
}

void GameObject::AddComponent(std::unique_ptr<Component> component) {
    assert(component != nullptr);
    m_components.push_back(std::move(component));
}

void GameObject::RemoveComponent(Component *component) {
    m_components.erase(
        std::remove_if(
            m_components.begin(), m_components.end(),
            [component](const auto &comp) { return comp.get() == component; }),
        m_components.end());
}
