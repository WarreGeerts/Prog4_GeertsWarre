#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Components/Components.h"
#include "Sound/servicelocator.h"

namespace dae {
    class GameActorCommand : public Command {
    public:
        explicit GameActorCommand(GameObject *actor) : m_pActor(actor) {}

    protected:
        GameObject *m_pActor{nullptr};
        [[nodiscard]] GameObject *GetActor() const { return m_pActor; }
    };


    //DEBUG COMMANDS
    class DieCommand : public GameActorCommand {
    public:
        explicit DieCommand(GameObject *actor, const EventId playerDieEvent)
        : GameActorCommand(actor), m_PlayerDieEvent(playerDieEvent) {}

        void Execute() override {
           EventManager::GetInstance().SendEvent(Event(m_PlayerDieEvent));
            servicelocator::GetSoundSystem().play(2,1.f);
        }
    private:
        EventId m_PlayerDieEvent{};
    };

    class ScoreCommandLow : public GameActorCommand {
    public:
        explicit ScoreCommandLow(GameObject *actor, const EventId playerScoreEvent)
            : GameActorCommand(actor), m_PlayerScoreEvent(playerScoreEvent) {}

        void Execute() override {
            EventManager::GetInstance().SendEvent(Event(m_PlayerScoreEvent).AddInt(10));
        }

    private:
        EventId m_PlayerScoreEvent{};
    };

    class ScoreCommandHigh : public GameActorCommand {
    public:
        explicit ScoreCommandHigh(GameObject *actor, const EventId playerScoreEvent)
            : GameActorCommand(actor), m_PlayerScoreEvent(playerScoreEvent) {}

        void Execute() override {
            EventManager::GetInstance().SendEvent(Event(m_PlayerScoreEvent).AddInt(100));
            servicelocator::GetSoundSystem().play(1,1.f);
        }

    private:
        EventId m_PlayerScoreEvent{};
    };
}
