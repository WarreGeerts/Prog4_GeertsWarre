#pragma once
#include "Component.h"
#include <string>
#include <SDL3/SDL_pixels.h>

#include "RenderComponent.h"

namespace dae {
    class Font;
    class Texture2D;
    class GameObject;
    class LivesDisplayComponent;
    class ScoreDisplayComponent;

    class TextComponent final : public Component {
    public:
        explicit TextComponent(GameObject *go);
        TextComponent(GameObject *go, std::string text, std::string fileName, int fontSize,
                      const SDL_Color &color = {255, 255, 255, 255});

        //-
        void Update() override;
        void Render() const override;
        void InspectorGUI() override;

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
        bool m_GuiUpdated{false};
        bool m_OverWriten{false};
        bool m_ExpensiveLoaded{false};

        std::string m_FileName;
        std::string m_text;
        std::string m_prevText{" "};
        SDL_Color m_color{255, 255, 255, 255};
        std::shared_ptr<Font> m_font;
        int m_fontSize{10};
        std::shared_ptr<Texture2D> m_textTexture{};

        RenderComponent *m_renderComponentRef{};
    };
}
