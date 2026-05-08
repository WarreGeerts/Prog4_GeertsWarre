#include "GameCommands.h"
namespace game {
    GoToLobbyCommand::GoToLobbyCommand(const ge::EventId goToLobbyEvent)
        : m_GoToLobbyEvent(goToLobbyEvent){}

    void GoToLobbyCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_GoToLobbyEvent));
    }

    UISelectionUpCommand::UISelectionUpCommand(const ge::EventId uiSelectionEvent)
        : m_upEvent(uiSelectionEvent){}

    void UISelectionUpCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_upEvent));
    }

    UISelectionDownCommand::UISelectionDownCommand(const ge::EventId uiSelectionEvent)
        : m_downEvent(uiSelectionEvent){}

    void UISelectionDownCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_downEvent));
    }
}
