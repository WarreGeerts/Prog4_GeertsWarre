#pragma once
#include <string>
#include "Component.h"

namespace ge {
    class Texture2D;

    struct Rectf {
        float x{}, y{}, w{}, h{};
    };

    struct SpriteFraming {
        int columns{}, rows{};
        float frameWidth{}, frameHeight{};
        float spacingX = 0.f, spacingY = 0.f;
        float offsetX = 0, offsetY = 0; // Added
    };

    class SpriteComponent final : public Component {
    public:
        explicit SpriteComponent(GameObject *go) : Component(go,"SpriteComponent") {}
        SpriteComponent(GameObject *go, const std::string &spritesheetPath, const SpriteFraming &spriteFrame, int frameIdx = 0);
        void SetSpritesheetPath(const std::string &spritesheetPath);
        void SetFrame(int frameIndex);
        void Update() override {}
        void Render() const override {}
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void InspectorGUI() override;

        [[nodiscard]] const Rectf &GetSrcRect() const { return m_SrcRect; }
        [[nodiscard]] const Rectf &GetDstRect() const { return m_DstRect; }
        std::shared_ptr<Texture2D> &GetTexture() { return m_Spritesheet; }
        void SetScale(float scaleX, float scaleY);
        void SetScale(const float uniformScale) { SetScale(uniformScale, uniformScale); }

    private:
        std::shared_ptr<Texture2D> m_Spritesheet;
        std::string m_FileName{};
        bool m_HasRenderComponent{false};

        Rectf m_SrcRect{0.f, 0.f, 0.f, 0.f};
        Rectf m_DstRect{0.f, 0.f, 0.f, 0.f};
        float m_ScaleX = 1.0f;
        float m_ScaleY = 1.0f;
        SpriteFraming m_SpriteFrame;
        int m_FrameIndex{};
    };
}
