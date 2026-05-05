#include "MoveCommand.h"

dae::MoveCommand::MoveCommand(GameObject *pActor, const glm::vec2 &direction, const float speed)
    : GameActorCommand(pActor), m_Direction(direction), m_Speed(speed) {}

void dae::MoveCommand::Execute() {
    if (!m_pActor) return;
    m_pActor->SetLocalPosition(
        m_pActor->GetWorldPosition() + glm::vec3(m_Direction, 0) * m_Speed * DeltaTime::GetInstance().Time());
}
