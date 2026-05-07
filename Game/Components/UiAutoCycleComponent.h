#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class UiAutoCycleComponent final : public Component {
    public:
        explicit UiAutoCycleComponent(GameObject *go) : Component(go, "UiAutoCycleComponent") {}
        UiAutoCycleComponent(GameObject *go, float waitTime);
        ~UiAutoCycleComponent() override = default;
        void SwitchInfo();
        void Update() override;
        void Render() const override {}
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        float m_WaitTime{3};
        float m_AccTime{0};
        int m_CurrentChildIndex{0};
    };
}
