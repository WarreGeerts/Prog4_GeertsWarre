#pragma once
#include "Component.h"

namespace dae {
    class CharacterControllerComponent final : public Component {
    public:
        CharacterControllerComponent(GameObject *go, float speed, bool keyboard, int controllerIdx = 0);
        ~CharacterControllerComponent() override = default;
        void Update() override{};
        void Render() const override{};
        void RenderGUI() override{};

    private:
        bool m_Keyboard;
        int m_ControllerIdx;
    };
}
