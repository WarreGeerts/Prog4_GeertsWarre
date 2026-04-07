#include "LivesComponent.h"

void dae::LivesComponent::InspectorGUI() {
    auto &em = EventManager::GetInstance();

    if (ImGui::InputInt("Lives", &m_Lives, 1, 1)) {
        em.SendEvent(Event(m_SendEventId).AddInt(m_Lives));
    }

    ImGui::Text("Triggers (Listening to):");
    if (ImGui::BeginListBox("##ListenList", ImVec2(-FLT_MIN, 150))) {
        for (int i = 0; i < static_cast<int>(m_ListenEventIds.size()); ++i) {
            std::string name = em.GetEventName(m_ListenEventIds[i]);

            ImGui::PushID(i);

            if (ImGui::Selectable(name.c_str(), false)) {}

            if (ImGui::BeginPopupContextItem("ItemContext")) {
                if (ImGui::MenuItem("Delete Listener")) {
                    em.DetachEvent(m_handles[i]);

                    m_ListenEventIds.erase(m_ListenEventIds.begin() + i);
                    m_handles.erase(m_handles.begin() + i);

                    ImGui::EndPopup();
                    ImGui::PopID();
                    break;
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        ImGui::EndListBox();
    }

    static EventId idToAdd = 0;
    const std::string previewName = em.GetEventName(idToAdd);

    ImGui::SetNextItemWidth(150);
    if (ImGui::BeginCombo("##AddEventCombo", previewName.c_str())) {
        for (auto const &[id, name]: em.GetRegisteredEvents()) {
            if (ImGui::Selectable(name.c_str(), id == idToAdd)) {
                idToAdd = id;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    if (ImGui::Button("Add Trigger")) {
        if (idToAdd != 0) {
            m_ListenEventIds.push_back(idToAdd);
            m_handles.push_back(em.AttachEvent(idToAdd, [this](const Event &) {
                PlayerDie();
            }));
        }
    }

    const std::string broadcastName = em.GetEventName(m_SendEventId);
    if (ImGui::BeginCombo("Broadcast Event", broadcastName.c_str())) {
        for (auto const &[id, name]: em.GetRegisteredEvents()) {
            if (ImGui::Selectable(name.c_str(), id == m_SendEventId)) {
                m_SendEventId = id;
            }
        }
        ImGui::EndCombo();
    }
}
