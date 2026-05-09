#include "GameCommands.h"

namespace game {
    SelectionCommand::SelectionCommand(const ge::EventId selectionEvent)
        : m_SelectionEvent(selectionEvent) {}

    void SelectionCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_SelectionEvent));
    }

    /*SinglePlayerCommand::SinglePlayerCommand(const ge::EventId SinglePlayerEvent)
        : m_SinglePlayerEvent(SinglePlayerEvent) {}

    void SinglePlayerCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_SinglePlayerEvent));
    }

    CoopCommand::CoopCommand(const ge::EventId CoopEvent)
        : m_CoopEvent(CoopEvent) {}

    void CoopCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_CoopEvent));
    }

    VersusCommand::VersusCommand(const ge::EventId VersusEvent)
        : m_VersusEvent(VersusEvent) {}

    void VersusCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_VersusEvent));
    }*/

    UISelectionUpCommand::UISelectionUpCommand(const ge::EventId uiSelectionEvent)
        : m_upEvent(uiSelectionEvent) {}

    void UISelectionUpCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_upEvent));
    }

    UISelectionDownCommand::UISelectionDownCommand(const ge::EventId uiSelectionEvent)
        : m_downEvent(uiSelectionEvent) {}

    void UISelectionDownCommand::Execute() {
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_downEvent));
    }
}
