#include "GameActorCommand.h"

namespace ge {
    GameActorCommand::GameActorCommand(GameObject *actor)
        : m_pActor(actor) {}

    GameObject *GameActorCommand::GetActor() const { return m_pActor; }

}
