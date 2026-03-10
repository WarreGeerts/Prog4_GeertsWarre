#pragma once
#include <string>
#include "Component.h"

namespace dae {
    class Texture2D;

    struct Rectf {
        float x{}, y{}, w{}, h{};
    };

    struct SpriteFraming {
        int columns{}, rows{};
        float frameWidth{}, frameHeight{};
        float spacingX = 0.f, spacingY = 0.f;
    };

    class SpriteComponent final : public Component {
    public:
        explicit SpriteComponent(GameObject *go) : Component(go) {}
        SpriteComponent(GameObject *go, const std::string &spritesheetPath, const SpriteFraming &spriteFrame, int frameIdx = 0);
        void SetSpritesheetPath(const std::string &spritesheetPath);
        void SetFrame(int frameIndex);
        void Update() override {}
        void Render() const override {}
        void RenderGUI() override {}
        [[nodiscard]] const Rectf &GetSrcRect() const { return m_SrcRect; }
        [[nodiscard]] const Rectf &GetDstRect() const { return m_DstRect; }
        std::shared_ptr<Texture2D> &GetTexture() { return m_Spritesheet; }
        void SetScale(float scaleX, float scaleY);
        void SetScale(const float uniformScale) { SetScale(uniformScale, uniformScale); }

    private:
        std::shared_ptr<Texture2D> m_Spritesheet;
        Rectf m_SrcRect{0.f, 0.f, 0.f, 0.f};
        Rectf m_DstRect{0.f, 0.f, 0.f, 0.f};
        float m_ScaleX = 1.0f;
        float m_ScaleY = 1.0f;
        int m_Columns{};
        int m_Rows{};
        float m_FrameWidth{};
        float m_FrameHeight{};
        float m_SpacingX{};
        float m_SpacingY{};
    };
}
