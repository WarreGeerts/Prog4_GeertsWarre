#pragma once
#include "Components/Component.h"
#include "Components/EngineComponents.h"
#include "Singletons/EventManager.h"

namespace dae {
    class ScoreDisplayComponent final : public Component {
    public:
        explicit ScoreDisplayComponent(GameObject *go) : Component(go, "ScoreDisplayComponent") {}
        explicit ScoreDisplayComponent(GameObject *go, EventId eventId);
        ~ScoreDisplayComponent() override;
        void SetHandle(EventId eventId);
        void SetRefTextComponent(TextComponent *component) { m_TextComponentRef = component; }
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

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
