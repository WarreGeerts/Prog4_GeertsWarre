#pragma once
#include "Component.h"

namespace dae {
    class FPSComponent final : public Component {
    public:
        explicit FPSComponent(GameObject *go);
        void Update() override;
        void Render() const override {}
        float GetFPS() const { return m_FPS; }

    private:
        float m_Time = 0.f;
        int m_Frames = 0;
        float m_FPS = 0;

        float m_IntervalTime{0.0f};
        int m_IntervalFrames{0};
        const float m_UpdateInterval{0.5f};
    };
}
