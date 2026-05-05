#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class LivesComponent final : public Component {
    public:
        explicit LivesComponent(GameObject *go) : Component(go, "LivesComponent") {}
        explicit LivesComponent(GameObject *go, const std::vector<EventId> &listenEventIds, EventId sendEventId);
        LivesComponent(GameObject *go, const std::vector<EventId> &listenEventIds, EventId sendEventId, int lives);
        ~LivesComponent() override;
        void Update() override {};
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void PlayerDie();

    private:
        std::vector<EventId> m_ListenEventIds{};
        std::vector<EventHandle> m_Handles{};
        EventId m_SendEventId{};
        int m_Lives{3};
    };
}
