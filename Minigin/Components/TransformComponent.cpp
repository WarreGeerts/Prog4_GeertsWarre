#include "TransformComponent.h"
#include "GameObject.h"

namespace dae {
    void TransformComponent::InspectorGUI() {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            float pos[3] = {m_position.x, m_position.y, m_position.z};
            if (ImGui::DragFloat2("Position", pos, 0.1f)) {
                m_gameObject->SetLocalPosition(glm::vec3(pos[0], pos[1], pos[2]));
            }
            ImGui::Text(" ");
        }
    }

    const glm::vec3 &TransformComponent::GetPosition() const {
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
