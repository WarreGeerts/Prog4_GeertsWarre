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

}
