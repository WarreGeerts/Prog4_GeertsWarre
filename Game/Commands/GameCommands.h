#pragma once
#include "GameObject.h"
#include "Commands/Command.h"
#include "Singletons/EventManager.h"

namespace game {
    class GoToLobbyCommand final : public ge::Command {
    public:
        explicit GoToLobbyCommand(ge::EventId goToLobbyEvent);
    private:
        void Execute() override;
        ge::EventId m_GoToLobbyEvent;
    };

    class UISelectionUpCommand final : public ge::Command {
    public:
        explicit UISelectionUpCommand(ge::EventId uiSelectionEvent);
    private:
        void Execute() override;
        ge::EventId m_upEvent;
    };

    class UISelectionDownCommand final : public ge::Command {
    public:
        explicit UISelectionDownCommand(ge::EventId uiSelectionEvent);
    private:
        void Execute() override;
        ge::EventId m_downEvent;
    };
}
