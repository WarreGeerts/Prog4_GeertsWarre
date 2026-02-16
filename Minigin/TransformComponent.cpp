#include "TransformComponent.h"

namespace dae {
    TransformComponent::TransformComponent(GameObject *go)
        : Component(go), m_transform(std::make_unique<Transform>()) {}

    TransformComponent::TransformComponent(GameObject *go, float x, float y)
        : Component(go), m_transform(std::make_unique<Transform>()) {
        m_transform->SetPosition(x, y, 0);
    }
}
