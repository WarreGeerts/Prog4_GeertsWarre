#pragma once
#include "Components/Component.h"
#include "Components/Components.h"
#include "Singletons/EventManager.h"

namespace dae {
    class ScoreDisplayComponent final : public Component {
    public:
        explicit ScoreDisplayComponent(GameObject *go) : Component(go, "ScoreDisplayComponent") {}

        explicit ScoreDisplayComponent(GameObject *go, const EventId eventId) : Component(go, "ScoreDisplayComponent"),
            m_ListenEventId(eventId) {
            SetHandle(m_ListenEventId);
        };

        ~ScoreDisplayComponent() override {
            EventManager::GetInstance().DetachEvent(m_Handle);
        }

        void SetHandle(const EventId eventId) {
            if (m_Handle.valid) {
                EventManager::GetInstance().DetachEvent(m_Handle);
            }

            m_Handle = EventManager::GetInstance().AttachEvent(
                eventId,
                [this](const Event &event) {
                    OnScoreIncrease(std::get<int>(event.args[0]));
                });
        }

        void SetRefTextComponent(TextComponent *component) { m_TextComponentRef = component; }
        void Update() override;
        void Render() const override{};
        void InspectorGUI() override;

    private:
        void OnScoreIncrease(const int score) { m_Score = score; };
        int m_Score{};
        int m_PrevScore{-1};
        TextComponent *m_TextComponentRef{};
        EventId m_ListenEventId{};
        EventHandle m_Handle{};
        std::string m_Text{"# Score: "};
    };
}
