#include "GameCommands.h"
namespace dae {
    GoToLobbyCommand::GoToLobbyCommand(EventId goToLobbyEvent)
        : m_GoToLobbyEvent(goToLobbyEvent){}

    void GoToLobbyCommand::Execute() {
        EventManager::GetInstance().SendEvent(Event(m_GoToLobbyEvent));
    }
}
