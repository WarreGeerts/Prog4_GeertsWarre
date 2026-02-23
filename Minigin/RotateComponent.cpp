#include "RotateComponent.h"
#include "DeltaTime.h"
#include "GameObject.h"
using namespace dae;

void RotateComponent::Update() {
    m_CurrentAngle += m_AngularSpeed * DeltaTime::GetInstance().Time();

    const glm::vec2 offset{m_Radius * cosf(m_CurrentAngle), m_Radius * sinf(m_CurrentAngle)};
    m_gameObject->SetLocalPosition({m_Center.x + offset.x, m_Center.y + offset.y, 0.f});
}
