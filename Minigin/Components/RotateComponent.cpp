#include "RotateComponent.h"
#include "Singletons/DeltaTime.h"
#include "GameObject.h"
using namespace dae;

void RotateComponent::Update() {
    if (!m_IsActive) return;


    glm::vec3 parentWorldPos {0,0,0};

    if (m_gameObject->GetParent())
        parentWorldPos = m_gameObject->GetParent()->GetWorldPosition();

    m_CurrentAngle += m_AngularSpeed * DeltaTime::GetInstance().Time();

    if (m_CurrentAngle > 2 * m_PI)
        m_CurrentAngle -= 2 * m_PI;

    if (m_CurrentAngle < 0)
        m_CurrentAngle += 2 * m_PI;

    const glm::vec2 offset{m_Radius * cosf(m_CurrentAngle), m_Radius * sinf(m_CurrentAngle)};
    const glm::vec3 targetWorldPos{parentWorldPos.x + offset.x, parentWorldPos.y + offset.y, 0.f};

    const glm::vec3 newLocalPos = targetWorldPos - parentWorldPos;
    m_gameObject->SetLocalPosition(newLocalPos);
}

nlohmann::ordered_json RotateComponent::Serialize() const {
    nlohmann::ordered_json data;
    data["radius"] = m_Radius;
    data["angular_speed"] = m_AngularSpeed;
    return data;
}

void RotateComponent::Deserialize(const nlohmann::ordered_json &data) {
    m_Radius = data.value("radius", 100.0f);
    m_AngularSpeed = data.value("angular_speed", 1.0f);
}

void RotateComponent::InspectorGUI() {
    float radius[1] = {m_Radius};
    if (ImGui::DragFloat("Radius", radius, 0.1f)) {
        m_Radius = radius[0];
    }
    float angularSpeed[1] = {m_AngularSpeed};
    if (ImGui::DragFloat("Angular Speed", angularSpeed, 0.01f)) {
        m_AngularSpeed = angularSpeed[0];
    }
}
