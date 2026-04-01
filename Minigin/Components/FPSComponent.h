#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace dae {
    class FPSComponent final : public Component {
    public:
        explicit FPSComponent(GameObject *go);
        void Update() override;
        void Render() const override;
        void SetRefTextComponent(TextComponent *component) {m_TextComponentRef = component; }

    private:
        float m_FPS{0.f};
        int m_Frames{0};
        float m_ElapsedTime{0.f};
        float m_TotalTime{0.f};
        TextComponent *m_TextComponentRef{};
    };
}
