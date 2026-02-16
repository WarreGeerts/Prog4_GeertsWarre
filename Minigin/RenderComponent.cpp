/*#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TransformComponent.h"
using namespace dae;
RenderComponent::RenderComponent(GameObject* go, const std::string& filename)
    : Component(go), m_texture(ResourceManager::GetInstance().LoadTexture(filename)) {}

void RenderComponent::Update() {}
void RenderComponent::Render() const {
    if (m_texture && m_gameObject->GetComponent<TransformComponent>()) {
        auto transform = m_gameObject->GetComponent<TransformComponent>()->GetTransform();
        Renderer::GetInstance().RenderTexture(*m_texture, transform->GetPosition().x, transform->GetPosition().y);
    }
}*/