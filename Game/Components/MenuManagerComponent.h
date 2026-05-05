#pragma once
#include "Components/Component.h"

namespace dae {
    class MenuManagerComponent final : public Component {
    public:
        explicit MenuManagerComponent(GameObject *go) : Component(go, "MenuManagerComponent") {}

        MenuManagerComponent(GameObject *go, const float waitTime) : Component(go, "MenuManagerComponent"),
                                                                     m_WaitTime(waitTime) {}

        ~MenuManagerComponent() override = default;
        void SwitchInfo();
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        float m_WaitTime{3};
        float m_AccTime{0};
        int m_CurrentChildIndex{0};
    };
}
