#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace dae {
    class FPSComponent final : public Component {
    public:
        explicit FPSComponent(GameObject *go);
        void Update() override;
        void Render() const override;
        float GetFPS() const { return m_FPS; }
        void SetPreviousFPS(float fps) { m_PreviousFPS = fps; }
        float GetPreviousFPS() const { return m_PreviousFPS; };
        void GetRefTextComponent(TextComponent *component) {m_TextComponentRef = component;}

    private:
        float m_FPS{0.f};
        int m_Frames{0};
        float m_PreviousFPS{0.f};
        float m_ElapsedTime{0.f};
        float m_TotalTime{0.f};
        TextComponent *m_TextComponentRef{};
    };
}
