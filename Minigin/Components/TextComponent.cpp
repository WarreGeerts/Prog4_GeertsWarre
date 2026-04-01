#include "TextComponent.h"
#include "Component.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "Singletons/Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <stdexcept>
using namespace dae;
//default constructor
TextComponent::TextComponent(GameObject *go)
    : Component(go,"TextComponent"), m_needsUpdate(false), m_text(" "), m_font(nullptr), m_textTexture(nullptr) {}

//extra one go constructor
TextComponent::TextComponent(GameObject *go, const std::string &text, std::shared_ptr<Font> font,
                             const SDL_Color &color)
    : Component(go,"TextComponent"), m_needsUpdate(true), m_text(text), m_color(color), m_font(std::move(font)),
      m_textTexture(nullptr) {}

void TextComponent::Update() {
    if (m_text != m_prevText) {
        m_needsUpdate = true;
    } else {
        m_needsUpdate = false;
    }

    if (m_needsUpdate) {
        const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
        if (surf == nullptr) {
            throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
        }
        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        if (texture == nullptr) {
            throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
        }
        SDL_DestroySurface(surf);
        m_textTexture = std::make_shared<Texture2D>(texture);
        m_needsUpdate = false;
        m_prevText = m_text;
    }
}

void TextComponent::Render() const {
    /*if (m_textTexture != nullptr) {
        auto &transform = m_gameObject->GetComponent<TransformComponent>()->GetPosition();
        Renderer::GetInstance().RenderTexture(*m_textTexture, transform.x, transform.y);
    }*/
    assert(m_renderComponentRef != nullptr);
    m_renderComponentRef->SetTexture(m_textTexture);
    m_renderComponentRef->Render();
}
