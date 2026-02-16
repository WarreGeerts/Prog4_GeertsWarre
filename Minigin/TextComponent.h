#pragma once
#include "Component.h"
#include <string>
#include <SDL3/SDL_pixels.h>

namespace dae {
    class Font;
    class Texture2D;
    class GameObject;

    class TextComponent : public Component {
    public:
        TextComponent(GameObject *go);
        TextComponent(GameObject *go, const std::string &text, std::shared_ptr<Font> font,
                      const SDL_Color &color = {255, 255, 255, 255});
        //-
        void Update() override;
        void Render() const override;
        //-
        void SetFont(std::shared_ptr<Font> font) { m_font = std::move(font); };

        void SetText(const std::string &text) {
            m_text = text;
            m_needsUpdate = true;
        }

        void SetColor(const SDL_Color &color) {
            m_color = color;
            m_needsUpdate = true;
        }

    private:
        bool m_needsUpdate{};
        std::string m_text{};
        SDL_Color m_color{255, 255, 255, 255};
        std::shared_ptr<Font> m_font{};
        std::shared_ptr<Texture2D> m_textTexture{};
    };
}
