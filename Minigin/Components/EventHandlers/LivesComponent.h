#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class LivesComponent final : public Component {
    public:
        explicit LivesComponent(GameObject *go, const EventId listenEventId, const EventId sendEventId)
            : Component(go, "LivesComponent"), m_SendEventId{sendEventId} {
            AttachEvents(listenEventId);
        }

        LivesComponent(GameObject *go, const EventId listenEventId, const EventId sendEventId, const int lives)
            : Component(go,"LivesComponent"), m_SendEventId{sendEventId}, m_Lives{lives} {
            AttachEvents(listenEventId);
        }

        ~LivesComponent() override {
            EventManager::GetInstance().DetachEvent(m_Handle);
        }

        void Update() override {};
        void Render() const override {};
        void InspectorGUI() override;

        void AttachEvents(const EventId listenEventId) {
            m_Handle = EventManager::GetInstance().AttachEvent(
                listenEventId,
                [this](const Event &) {
                    PlayerDie();
                }
            );
        }

        void PlayerDie() {
            --m_Lives;
            EventManager::GetInstance().SendEvent(Event(m_SendEventId).AddInt(m_Lives));
        }

    private:
        EventId m_SendEventId;
        EventHandle m_Handle;
        int m_Lives{3};

    };
}
