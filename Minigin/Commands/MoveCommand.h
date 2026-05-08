#pragma once
#include "GameActorCommand.h"
#include <glm/glm.hpp>
#include "Singletons/DeltaTime.h"

namespace ge {
    class MoveCommand final : public GameActorCommand {
    public:
        MoveCommand(GameObject *pActor, const glm::vec2 &direction, const float speed);
        void Execute() override;

    private:
        glm::vec2 m_Direction;
        float m_Speed;
    };
}
