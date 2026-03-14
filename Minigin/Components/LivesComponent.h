#pragma once
#include "Component.h"
#include "EventManager.h"

namespace dae {
    class LivesComponent final : public Component {
    public:
        explicit LivesComponent(GameObject *go, const EventId eventId) : Component(go), m_EventId{eventId} {}

        LivesComponent(GameObject *go, const EventId eventId, const int lives) : Component(go), m_EventId{eventId},
            m_Lives{lives} {}

        void Update() override {};
        void Render() const override {};
        void RenderGUI() override {};

        void PlayerDie() {
            --m_Lives;
            EventManager::GetInstance().SendEvent(Event(m_EventId).AddInt(m_Lives));
        }

    private:
        EventId m_EventId;
        int m_Lives{3};
    };
}
