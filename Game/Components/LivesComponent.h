#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace game {
    class LivesComponent final : public ge::Component {
    public:
        explicit LivesComponent(ge::GameObject *go) : Component(go, "LivesComponent") {}
        explicit LivesComponent(ge::GameObject *go, const std::vector<ge::EventId> &listenEventIds, ge::EventId sendEventId);
        LivesComponent(ge::GameObject *go, const std::vector<ge::EventId> &listenEventIds, ge::EventId sendEventId, int lives);
        ~LivesComponent() override;
        void Update() override {};
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void PlayerDie();

    private:
        std::vector<ge::EventId> m_ListenEventIds{};
        std::vector<ge::EventHandle> m_Handles{};
        ge::EventId m_SendEventId{};
        int m_Lives{3};
    };
}
