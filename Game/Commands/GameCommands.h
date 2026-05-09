#pragma once
#include "GameObject.h"
#include "Commands/Command.h"
#include "Singletons/EventManager.h"

namespace game {
    class SelectionCommand final : public ge::Command {
    public:
        explicit SelectionCommand(ge::EventId selectionEvent);
    private:
        void Execute() override;
        ge::EventId m_SelectionEvent;
    };

    /*class SinglePlayerCommand final : public ge::Command {
    public:
        explicit SinglePlayerCommand(ge::EventId SinglePlayerEvent);
    private:
        void Execute() override;
        ge::EventId m_SinglePlayerEvent;
    };

    class CoopCommand final : public ge::Command {
    public:
        explicit CoopCommand(ge::EventId CoopEvent);
    private:
        void Execute() override;
        ge::EventId m_CoopEvent;
    };

    class VersusCommand final : public ge::Command {
    public:
        explicit VersusCommand(ge::EventId VersusEvent);
    private:
        void Execute() override;
        ge::EventId m_VersusEvent;
    };*/

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
