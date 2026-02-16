#pragma once
#include "Component.h"
#include <string>

namespace dae {
    class TextObject;
    class Font;
    class FPSComponent : public Component {
    public:
        FPSComponent(GameObject* go);
        void Update() override;
        void Render() const override {}
    private:
        float m_time = 0.f;
        int m_frames = 0;
        int m_fps = 0;
        std::string m_text;
    };
}