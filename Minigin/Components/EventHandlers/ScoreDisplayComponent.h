#pragma once
#include "Components/Component.h"
#include "Components/Components.h"
#include "Singletons/EventManager.h"

namespace dae {
    class ScoreDisplayComponent final : public Component {
    public:
        explicit ScoreDisplayComponent(GameObject *go, const EventId eventId) : Component(go,"ScoreDisplayComponent") {
            m_Handle = EventManager::GetInstance().AttachEvent(
                eventId,
                [this](const Event &event) {
                    OnScoreIncrease(event.args[0].i);
                });
        };

        ~ScoreDisplayComponent() override {
            EventManager::GetInstance().DetachEvent(m_Handle);
        }

        void SetRefTextComponent(TextComponent *component) {m_TextComponentRef = component;}

        void Update() override;
        void Render() const override;
        void InspectorGUI() override;

    private:
        void OnScoreIncrease(const int score) { m_Score = score; };
        int m_Score{};
        TextComponent *m_TextComponentRef{};
        EventHandle m_Handle{};
        std::string m_Text{"# Score: "};

    };
}
