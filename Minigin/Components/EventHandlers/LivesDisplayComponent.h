#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"
#include "Components/Components.h"

namespace dae {
    class LivesDisplayComponent final : public Component {
    public:
        explicit LivesDisplayComponent(GameObject *go) : Component(go, "LivesDisplayComponent") {}

        explicit LivesDisplayComponent(GameObject *go, const EventId eventId) : Component(go, "LivesDisplayComponent"),
            m_ListenEventId(eventId) {
            SetHandle(m_ListenEventId);
        };

        ~LivesDisplayComponent() override {
            EventManager::GetInstance().DetachEvent(m_Handle);
        };

        void SetHandle(const EventId eventId) {
            if (m_Handle.valid) {
                EventManager::GetInstance().DetachEvent(m_Handle);
            }

            m_Handle = EventManager::GetInstance().AttachEvent(
                eventId,
                [this](const Event &event) {
                    OnPlayerDied(std::get<int>(event.args[0]));
                });
        }

        void SetRefTextComponent(TextComponent *component) { m_TextComponentRef = component; }
        void Update() override;;
        void Render() const override{};
        void InspectorGUI() override;

    private:
        void OnPlayerDied(const int lives) { m_Lives = lives; };
        int m_Lives{3};
        int m_PrevLives{-1};
        TextComponent *m_TextComponentRef{};
        EventId m_ListenEventId{};
        EventHandle m_Handle{};
        std::string m_Text{"# Lives: "};
    };
}
