#include "Component.h"
#include "GameObject.h"
namespace ge {
    int Component::s_NextId = 0;

    void Component::CheckActive() const {
        if (!m_IsActive) {
            ImGui::BeginDisabled(true);
        }
    }

    void Component::EndCheckActive() const {
        if (!m_IsActive) {
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::SetTooltip("Component not active");
            }
            ImGui::EndDisabled();
        }
    }

    bool Component::NeedsUpdate() {
        if (m_LastSeenVersion != m_gameObject->GetComponentListVersion()) {
            m_LastSeenVersion = m_gameObject->GetComponentListVersion();
            return true;
        }
        return false;
    }
}