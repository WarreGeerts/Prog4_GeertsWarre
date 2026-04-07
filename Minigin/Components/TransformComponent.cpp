#include "TransformComponent.h"
#include "GameObject.h"

namespace dae {
    void TransformComponent::InspectorGUI() {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            const glm::vec3 localPos = m_gameObject->GetLocalPosition();
            float lPos[2] = {localPos.x, localPos.y};

            if (ImGui::DragFloat2("Local Position", lPos, 0.1f)) {
                m_gameObject->SetLocalPosition(glm::vec3(lPos[0], lPos[1], 0.0f));
            }

            const glm::vec3 worldPos = m_gameObject->GetWorldPosition();
            float wPos[2] = {worldPos.x, worldPos.y};

            ImGui::BeginDisabled();
            if (ImGui::DragFloat2("World Position", wPos, 0.1f)) {
                m_gameObject->SetLocalPosition(glm::vec3(wPos[0], wPos[1], 0.0f));
            }
            ImGui::EndDisabled();
            if (m_gameObject->GetParent() != nullptr) {
                ImGui::TextDisabled("(Relative to parent: %s)", m_gameObject->GetParent()->GetName().c_str());
            }
        }
        ImGui::Text(" ");
    }

    const glm::vec3 &TransformComponent::GetPosition() const {
        return m_gameObject->GetWorldPosition();
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
