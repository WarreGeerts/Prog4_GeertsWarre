#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
using namespace dae;
GameObject::~GameObject() = default;
void GameObject::Update() {
    for (auto &component : m_components) {
        component->Update();
    }
}

void GameObject::Render() const {
    const auto &pos = this->GetComponent<TransformComponent>()->GetPosition();
    Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void GameObject::SetTexture(const std::string &filename) {
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

/*void GameObject::SetPosition(float x, float y) {
    m_transform.SetPosition(x, y, 0.0f);
}*/

void GameObject::AddComponent(std::unique_ptr<Component> component) {
    component->m_gameObject = this;
    m_components.push_back(std::move(component));
}

void GameObject::RemoveComponent(Component *component) {
    m_components.erase(
        std::remove_if(
            m_components.begin(), m_components.end(),
            [component](const auto &comp) { return comp.get() == component; }),
        m_components.end());
}

template<typename T>
T *GameObject::GetComponent() const {
    for (const auto &component: m_components) {
        if (auto *casted_comp = dynamic_cast<T *>(component.get())) {
            return casted_comp;
        }
    }
    return nullptr;
}
