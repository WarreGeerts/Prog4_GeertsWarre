#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
using namespace dae;

RenderComponent::RenderComponent(GameObject *go)
    : Component(go) {}

RenderComponent::RenderComponent(GameObject *go, const std::string &filename)
    : Component(go) { SetTexture(filename); }

void RenderComponent::Update() {}

void RenderComponent::Render() const {
    if (m_texture && m_gameObject->GetComponent<TransformComponent>()) {
        const auto& transform = m_gameObject->GetComponent<TransformComponent>()->GetPosition();
        Renderer::GetInstance().RenderTexture(*m_texture, transform.x, transform.y);
    }
}

void RenderComponent::SetTexture(const std::string &filename) {
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

