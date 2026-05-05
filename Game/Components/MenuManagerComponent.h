#pragma once
#include "Components/Component.h"

namespace dae {
    class MenuManagerComponent final : public Component {
    public:
        explicit MenuManagerComponent(GameObject *go) : Component(go, "MenuManagerComponent") {}
        ~MenuManagerComponent() override;
        void Update() override;
        void Render() const override{};
        void InspectorGUI() override{};
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        float m_WaitTime{3};
        float m_AccTime{};

    };
}
