#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Components/Components.h"

namespace dae {
    class GameActorCommand : public Command {
    public:
        explicit GameActorCommand(GameObject *actor) : m_pActor(actor) {}

    protected:
        GameObject *m_pActor{nullptr};
        [[nodiscard]] GameObject *GetActor() const { return m_pActor; }
    };

    class DieCommand : public GameActorCommand {
    public:
        explicit DieCommand(GameObject *actor) : GameActorCommand(actor) {}

        void Execute() override {
            if (m_pActor->GetComponent<LivesComponent>()) {
                m_pActor->GetComponent<LivesComponent>()->PlayerDie();
            }
        }
    };
}
