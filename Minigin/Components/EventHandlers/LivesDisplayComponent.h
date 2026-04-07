#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"
#include "Components/Components.h"

namespace dae {
    class LivesDisplayComponent final : public Component {
    public:
        explicit LivesDisplayComponent(GameObject *go, const EventId eventId) : Component(go,"LivesDisplayComponent") {
            m_Handle = EventManager::GetInstance().AttachEvent(
                eventId,
                [this](const Event& event) {
                    OnPlayerDied(event.args[0].i);
                });
        };

        ~LivesDisplayComponent() override {
            EventManager::GetInstance().DetachEvent(m_Handle);
        };

        void SetRefTextComponent(TextComponent *component) {m_TextComponentRef = component;}

        void Update() override;;
        void Render() const override;
        void InspectorGUI() override;

    private:
        void OnPlayerDied(const int lives){m_Lives = lives;};

        int m_Lives{3};
        TextComponent *m_TextComponentRef{};
        EventHandle m_Handle{};
        std::string m_Text{"# Lives: "};

    };
}
