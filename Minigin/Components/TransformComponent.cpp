#include "TransformComponent.h"
#include "GameObject.h"

namespace dae {
    const glm::vec3 & TransformComponent::GetPosition() const {
        m_gameObject->UpdateWorldPosition();
        return m_position;
    }

    void TransformComponent::SetPosition(float x, float y, float z) {
        m_position.x = x;
        m_position.y = y;
        m_position.z = z;
    }

    void TransformComponent::SetPosition(const glm::vec3 &position) {
        m_position = position;
    }
}
