#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"
#include "Components/Components.h"

namespace dae {
    class ScoreDisplayComponent final : public Component {
    public:
        explicit ScoreDisplayComponent(GameObject *go, const EventId eventId) : Component(go) {
            EventManager::GetInstance().AttachEvent(
                eventId,
                [this](const Event &event) {
                    OnScoreIncrease(event.args[0].i);
                });
        };

        void SetRefTextComponent(TextComponent *component) {m_TextComponentRef = component;}

        void Update() override {};
        void Render() const override;
        void RenderGUI() override {};

    private:
        void OnScoreIncrease(const int score) { m_Score = score; };
        int m_Score{};
        TextComponent *m_TextComponentRef{};
    };
}
