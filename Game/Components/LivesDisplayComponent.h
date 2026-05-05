#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"
#include "Components/EngineComponents.h"

namespace dae {
    class LivesDisplayComponent final : public Component {
    public:
        explicit LivesDisplayComponent(GameObject *go) : Component(go, "LivesDisplayComponent") {}
        explicit LivesDisplayComponent(GameObject *go, EventId eventId);
        ~LivesDisplayComponent() override;
        void SetHandle(EventId eventId);
        void SetRefTextComponent(TextComponent *component) { m_TextComponentRef = component; }
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        void OnPlayerDied(const int lives) { m_Lives = lives; };
        int m_MaxLives{3};
        int m_Lives{m_MaxLives};
        int m_PrevLives{-1};
        TextComponent *m_TextComponentRef{};
        EventId m_ListenEventId{};
        EventHandle m_Handle{};
        std::string m_Text{"# Lives: "};
    };
}
