#pragma once
#include "Components/Component.h"
#include "Components/EngineComponents.h"
#include "Singletons/EventManager.h"

namespace game {
    class ScoreDisplayComponent final : public ge::Component {
    public:
        explicit ScoreDisplayComponent(ge::GameObject *go) : Component(go, "ScoreDisplayComponent") {}
        explicit ScoreDisplayComponent(ge::GameObject *go, ge::EventId eventId);
        ~ScoreDisplayComponent() override;
        void SetHandle(ge::EventId eventId);
        void SetRefTextComponent(ge::TextComponent *component) { m_TextComponentRef = component; }
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        bool IsOverwritingText() override { return true; }

    private:
        void OnScoreIncrease(const int score) { m_Score = score; };
        int m_Score{};
        int m_PrevScore{-1};
        ge::TextComponent *m_TextComponentRef{};
        ge::EventId m_ListenEventId{};
        ge::EventHandle m_Handle{};
        std::string m_Text{"# Score: "};
    };
}
