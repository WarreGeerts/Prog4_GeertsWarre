#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class LivesComponent final : public Component {
    public:
        explicit LivesComponent(GameObject *go) : Component(go, "LivesComponent") {}

        explicit LivesComponent(GameObject *go, const std::vector<EventId> &listenEventIds, const EventId sendEventId)
            : Component(go, "LivesComponent"), m_ListenEventIds(listenEventIds), m_SendEventId{sendEventId} {
            for (const auto &eventId: m_ListenEventIds) {
                m_handles.emplace_back(EventManager::GetInstance().AttachEvent(
                    eventId,
                    [this](const Event &) {
                        PlayerDie();
                    }
                ));
            }
        }

        LivesComponent(GameObject *go, const std::vector<EventId> &listenEventIds, const EventId sendEventId,
                       const int lives)
            : Component(go, "LivesComponent"), m_ListenEventIds(listenEventIds), m_SendEventId{sendEventId},
              m_Lives{lives} {
            for (const auto &eventId: m_ListenEventIds) {
                m_handles.emplace_back(EventManager::GetInstance().AttachEvent(
                    eventId,
                    [this](const Event &) {
                        PlayerDie();
                    }
                ));
            }
        }

        ~LivesComponent() override {
            for (const auto &handle: m_handles) {
                EventManager::GetInstance().DetachEvent(handle);
            }
        }


        void Update() override {};
        void Render() const override {};
        void InspectorGUI() override;

        void PlayerDie() {
            --m_Lives;
            EventManager::GetInstance().SendEvent(Event(m_SendEventId).AddInt(m_Lives));
        }

    private:
        std::vector<EventId> m_ListenEventIds{};
        std::vector<EventHandle> m_handles{};
        EventId m_SendEventId{};
        int m_Lives{3};
    };
}
