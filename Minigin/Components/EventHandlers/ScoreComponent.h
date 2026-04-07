#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class ScoreComponent final : public Component {
    public:
        explicit ScoreComponent(GameObject *go, const std::vector<EventId> &listenEventIds, const EventId sendEventId)
            : Component(go,"ScoreComponent"), m_ListenEventIds{listenEventIds}, m_SendEventId{sendEventId} {
            for (const auto &eventId: m_ListenEventIds) {
                m_handles.emplace_back(EventManager::GetInstance().AttachEvent(
                    eventId,
                    [this](const Event &event) { IncreaseScore(event.args[0].i); }
                ));
            }
        }

        ~ScoreComponent() override {
            for (const auto &handle: m_handles) {
                EventManager::GetInstance().DetachEvent(handle);
            }
        }

        void Update() override {};
        void Render() const override {};
        void InspectorGUI() override;

        void IncreaseScore(const int amount) {
            m_Score += amount;
            EventManager::GetInstance().SendEvent(Event(m_SendEventId).AddInt(m_Score));
        }

    private:
        std::vector<EventId> m_ListenEventIds;
        std::vector<EventHandle> m_handles;
        EventId m_SendEventId;
        int m_Score{};
    };
}
