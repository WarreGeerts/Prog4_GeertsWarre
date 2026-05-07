#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class ScoreComponent final : public Component {
    public:
        explicit ScoreComponent(GameObject *go) : Component(go, "ScoreComponent") {}
        explicit ScoreComponent(GameObject *go, const std::vector<EventId> &listenEventIds, EventId sendEventId);
        ~ScoreComponent() override;
        void Update() override {};
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void IncreaseScore(int amount);

    private:
        std::vector<EventId> m_ListenEventIds{};
        EventId m_SendEventId{};
        std::vector<EventHandle> m_Handles;
        int m_Score{};
    };
}
