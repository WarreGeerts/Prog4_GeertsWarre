#include "GameActorCommand.h"

namespace ge {
    GameActorCommand::GameActorCommand(GameObject *actor)
        : m_pActor(actor) {}

    GameObject *GameActorCommand::GetActor() const { return m_pActor; }

    DieCommand::DieCommand(GameObject *actor, const EventId playerDieEvent)
        : GameActorCommand(actor), m_PlayerDieEvent(playerDieEvent) {}

    void DieCommand::Execute() {
        EventManager::GetInstance().SendEvent(Event(m_PlayerDieEvent));
        ServiceLocator::GetSoundSystem().Play(2, 1.f);
    }

    ScoreCommandLow::ScoreCommandLow(GameObject *actor, const EventId playerScoreEvent)
        : GameActorCommand(actor), m_PlayerScoreEvent(playerScoreEvent) {}

    void ScoreCommandLow::Execute() {
        EventManager::GetInstance().SendEvent(Event(m_PlayerScoreEvent).AddInt(10));
    }

    ScoreCommandHigh::ScoreCommandHigh(GameObject *actor, const EventId playerScoreEvent)
        : GameActorCommand(actor), m_PlayerScoreEvent(playerScoreEvent) {}

    void ScoreCommandHigh::Execute() {
        EventManager::GetInstance().SendEvent(Event(m_PlayerScoreEvent).AddInt(100));
        ServiceLocator::GetSoundSystem().Play(1, 1.f);
    }
}
