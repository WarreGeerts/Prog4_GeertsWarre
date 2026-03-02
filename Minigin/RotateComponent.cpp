#include "RotateComponent.h"
#include "DeltaTime.h"
#include "GameObject.h"
using namespace dae;

void RotateComponent::Update() {
    glm::vec3 parentWorldPos = m_gameObject->GetParent()->GetWorldPosition();
    m_CurrentAngle += m_AngularSpeed * DeltaTime::GetInstance().Time();

    if (m_CurrentAngle > 2 * m_PI)
        m_CurrentAngle -= 2 * m_PI;

    if (m_CurrentAngle < 0)
        m_CurrentAngle += 2 * m_PI;

    glm::vec2 offset{m_Radius * cosf(m_CurrentAngle), m_Radius * sinf(m_CurrentAngle)};
    glm::vec3 targetWorldPos{parentWorldPos.x + offset.x, parentWorldPos.y + offset.y, 0.f};

    glm::vec3 newLocalPos = targetWorldPos - parentWorldPos;
    m_gameObject->SetLocalPosition(newLocalPos);
}
