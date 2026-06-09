#include "MoveCommand.h"

#include "Components/CharacterControllerComponent.h"

namespace ge {
    MoveCommand::MoveCommand(GameObject *pActor, const glm::vec2 &direction, const float speed)
        : GameActorCommand(pActor), m_Direction(direction), m_Speed(speed) {}

    void MoveCommand::Execute() {
        if (!m_pActor) return;

        glm::vec3 displacement = glm::vec3(m_Direction, 0) * m_Speed * DeltaTime::GetInstance().Time();
        m_pActor->AddIntendedMovement(displacement);
    }
}
