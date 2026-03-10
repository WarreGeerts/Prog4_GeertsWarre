#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
using namespace dae;

RenderComponent::RenderComponent(GameObject *go)
    : Component(go) {}

RenderComponent::RenderComponent(GameObject *go, const std::string &filename)
    : RenderComponent(go) { SetTexture(filename); }

void RenderComponent::Update() {
    if (!m_expensiveLoaded) {
        m_transform = m_gameObject->GetComponent<TransformComponent>();
        m_sprite = m_gameObject->GetComponent<SpriteComponent>();
        m_expensiveLoaded = true;
    }
}

void RenderComponent::Render() const {
    if (!m_transform) return;

    const float posX{m_transform->GetPosition().x};
    const float posY{m_transform->GetPosition().y};

    // Check for SpriteComponent
    if (m_sprite) {
        const SDL_FRect src{
            m_sprite->GetSrcRect().x, m_sprite->GetSrcRect().y,
            m_sprite->GetSrcRect().w, m_sprite->GetSrcRect().h
        };
        const SDL_FRect dst{
            posX + m_sprite->GetDstRect().x, posY + m_sprite->GetDstRect().y,
            m_sprite->GetDstRect().w, m_sprite->GetDstRect().h
        };
        Renderer::GetInstance().RenderTextureRegion(*m_sprite->GetTexture(), &src, &dst);
    } else {
        if (!m_texture) return;
        Renderer::GetInstance().RenderTexture(*m_texture, posX, posY);
    }
}

void RenderComponent::SetTexture(const std::string &filename) {
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
