#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Singletons/EventManager.h"
#include "Sound/ServiceLocator.h"

namespace ge {
    class GameActorCommand : public Command {
    public:
        explicit GameActorCommand(GameObject *actor);

    protected:
        GameObject *m_pActor{nullptr};
        [[nodiscard]] GameObject *GetActor() const;
    };

    //DEBUG COMMANDS
    class DieCommand : public GameActorCommand {
    public:
        explicit DieCommand(GameObject *actor, EventId playerDieEvent);
        void Execute() override;

    private:
        EventId m_PlayerDieEvent{};
    };

    class ScoreCommandLow : public GameActorCommand {
    public:
        explicit ScoreCommandLow(GameObject *actor, EventId playerScoreEvent);
        void Execute() override;

    private:
        EventId m_PlayerScoreEvent{};
    };

    class ScoreCommandHigh : public GameActorCommand {
    public:
        explicit ScoreCommandHigh(GameObject *actor, EventId playerScoreEvent);
        void Execute() override;

    private:
        EventId m_PlayerScoreEvent{};
    };
}
