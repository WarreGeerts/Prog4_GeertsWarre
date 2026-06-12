#pragma once
#include "Events.h"
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace game {
    class DamageComponent final : public ge::Component {
    public:
        explicit DamageComponent(ge::GameObject* go) : Component(go, "DamageComponent") {
            m_SendEventId1 = EventRegistry::P1_DMG;
            m_SendEventId2 = EventRegistry::P2_DMG;
        };
        void Update() override;
        void Render() const override{};
        void InspectorGUI() override;
        nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        ge::EventId m_SendEventId1{};
        ge::EventId m_SendEventId2{};

    };
}
