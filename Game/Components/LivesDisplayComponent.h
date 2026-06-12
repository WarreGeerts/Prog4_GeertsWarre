#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"
#include "Components/EngineComponents.h"

namespace game {
    enum class DisplayMode { Text, Sprites };
    class LivesDisplayComponent final : public ge::Component {
    public:
        explicit LivesDisplayComponent(ge::GameObject *go) : Component(go, "LivesDisplayComponent") {}
        explicit LivesDisplayComponent(ge::GameObject *go, ge::EventId eventId);
        ~LivesDisplayComponent() override;
        void SetHandle(ge::EventId eventId);
        void SetRefTextComponent(ge::TextComponent *component) { m_TextComponentRef = component; }
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        bool IsOverwritingText() override {return true;}

    private:
        void OnPlayerDied(const int lives);
        int m_MaxLives{3};
        int m_Lives{m_MaxLives};
        int m_PrevLives{-1};
        ge::TextComponent *m_TextComponentRef{};
        ge::EventId m_ListenEventId{};
        ge::EventHandle m_Handle{};
        std::string m_Text{"# Lives: "};
        DisplayMode m_DisplayMode = DisplayMode::Text;
        std::vector<ge::GameObject*> m_LifeSprites;
    };
}
