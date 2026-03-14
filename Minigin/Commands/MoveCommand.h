#pragma once
#include "GameActorCommand.h"
#include <glm/glm.hpp>
#include "Singletons/DeltaTime.h"

namespace dae {
    class MoveCommand final : public GameActorCommand {
    public:
        MoveCommand(GameObject *pActor, const glm::vec2 &direction, const float speed)
            : GameActorCommand(pActor)
              , m_Direction(direction)
              , m_Speed(speed) {}

        void Execute() override {
            if (!m_pActor) return;
            m_pActor->SetLocalPosition(m_pActor->GetWorldPosition() + glm::vec3(m_Direction, 0) * m_Speed * DeltaTime::GetInstance().Time());
        }

    private:
        glm::vec2 m_Direction;
        float m_Speed;
    };
}
