#pragma once
#include "Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class ScoreComponent final : public Component {
    public:
        explicit ScoreComponent(GameObject *go, const EventId eventId) : Component(go), m_EventId{eventId} {};
        void Update() override {};
        void Render() const override {};
        void RenderGUI() override {};

        void IncreaseScore(const int amount) {
            m_Score += amount;
            EventManager::GetInstance().SendEvent(Event(m_EventId).AddInt(m_Score));
        }

    private:
        EventId m_EventId;
        int m_Score{};
    };
}
