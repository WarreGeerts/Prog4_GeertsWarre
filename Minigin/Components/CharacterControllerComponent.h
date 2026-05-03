#pragma once
#include "Component.h"

namespace dae {
    class CharacterControllerComponent final : public Component {
    public:
        explicit CharacterControllerComponent(GameObject *go);
        CharacterControllerComponent(GameObject *go, float speed, bool keyboard);
        ~CharacterControllerComponent() override = default;
        void Update() override{};
        void Render() const override{};
        void InspectorGUI() override;
        void AddControlBindings(float speed, bool keyboard);
        void RemoveControlBindings();
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        bool m_Keyboard;
        float m_Speed{50.f};
        bool m_Bound{false};
    };
}
