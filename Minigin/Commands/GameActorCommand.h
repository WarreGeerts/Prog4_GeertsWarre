#pragma once
#include "Command.h"
#include "GameObject.h"

namespace dae {
    class GameActorCommand : public Command {
    public:
        explicit GameActorCommand(GameObject *actor) : m_pActor(actor) {}
        virtual ~GameActorCommand() = default;

    protected:
        GameObject *m_pActor{nullptr};
        [[nodiscard]] GameObject *GetActor() const { return m_pActor; }
    };
}
