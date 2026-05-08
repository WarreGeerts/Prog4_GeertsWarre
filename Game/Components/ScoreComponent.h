#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace game {
    class ScoreComponent final : public ge::Component {
    public:
        explicit ScoreComponent(ge::GameObject *go) : Component(go, "ScoreComponent") {}
        explicit ScoreComponent(ge::GameObject *go, const std::vector<ge::EventId> &listenEventIds, ge::EventId sendEventId);
        ~ScoreComponent() override;
        void Update() override {};
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void IncreaseScore(int amount);

    private:
        std::vector<ge::EventId> m_ListenEventIds{};
        ge::EventId m_SendEventId{};
        std::vector<ge::EventHandle> m_Handles;
        int m_Score{};
    };
}
