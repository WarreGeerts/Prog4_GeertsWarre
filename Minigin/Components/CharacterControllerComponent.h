#pragma once
#include "Component.h"

namespace dae {
    class CharacterControllerComponent final : public Component {
    public:
        CharacterControllerComponent(GameObject *go);
        CharacterControllerComponent(GameObject *go, float speed, bool keyboard);
        ~CharacterControllerComponent() override = default;
        void Update() override{};
        void Render() const override{};
        void InspectorGUI() override;

    private:
        bool m_Keyboard;
    };
}
