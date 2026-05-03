#include "ScoreComponent.h"

void dae::ScoreComponent::InspectorGUI() {
    auto &em = EventManager::GetInstance();

    if (ImGui::InputInt("Current Score", &m_Score, 1, 10)) {
        em.SendEvent(Event(m_SendEventId).AddInt(m_Score));
    }

    ImGui::Text("Triggers (Listening to):");
    if (ImGui::BeginListBox("##ListenList##SC", ImVec2(-FLT_MIN, 150))) {
        for (int i = 0; i < static_cast<int>(m_ListenEventIds.size()); ++i) {
            std::string name = em.GetEventName(m_ListenEventIds[i]);

            ImGui::PushID(i);

            if (ImGui::Selectable(name.c_str(), false)) {}

            if (ImGui::BeginPopupContextItem("ItemContext##SC")) {
                if (ImGui::MenuItem("Delete Listener##SC")) {
                    em.DetachEvent(m_Handles[i]);

                    m_ListenEventIds.erase(m_ListenEventIds.begin() + i);
                    m_Handles.erase(m_Handles.begin() + i);

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

    ImGui::SetNextItemWidth(200);
    if (ImGui::BeginCombo("##AddEventCombo##SC", previewName.c_str())) {
        for (auto const &[id, name]: em.GetRegisteredEvents()) {
            if (ImGui::Selectable(name.c_str(), id == idToAdd)) {
                idToAdd = id;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    if (ImGui::Button("Add Trigger##SC")) {
        if (idToAdd != 0) {
            m_ListenEventIds.push_back(idToAdd);
            m_Handles.push_back(em.AttachEvent(idToAdd, [this](const Event &e) {
                IncreaseScore(std::get<int>(e.args[0]));
            }));
        }
    }

    const std::string broadcastName = em.GetEventName(m_SendEventId);
    if (ImGui::BeginCombo("Broadcast Event##SC", broadcastName.c_str())) {
        for (auto const &[id, name]: em.GetRegisteredEvents()) {
            if (ImGui::Selectable(name.c_str(), id == m_SendEventId)) {
                m_SendEventId = id;
            }
        }
        ImGui::EndCombo();
    }
}
