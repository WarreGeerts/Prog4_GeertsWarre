#pragma once
#include "Component.h"
#include "Transform.h"

namespace dae {
    class TransformComponent : public Component {
    public:
        TransformComponent(GameObject *go);

        TransformComponent(GameObject *go, float x, float y);

        void Update() override;
        void Render() const override;

        const glm::vec3 &GetPosition() const { return m_transform->GetPosition(); }
        void SetPosition(float x, float y, float z = 0) const { m_transform->SetPosition(x, y, z); }
        void SetPosition(const glm::vec3& position) const { m_transform->SetPosition(position); }

    private:
        std::unique_ptr<Transform> m_transform;
    };
}
