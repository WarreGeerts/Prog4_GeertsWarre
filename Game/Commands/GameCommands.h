#pragma once
#include "GameObject.h"
#include "Commands/Command.h"
#include "Singletons/EventManager.h"

namespace dae {
    class GoToLobbyCommand final : public Command {
    public:
        explicit GoToLobbyCommand(EventId goToLobbyEvent);
    private:
        void Execute() override;
        EventId m_GoToLobbyEvent;
    };
}
