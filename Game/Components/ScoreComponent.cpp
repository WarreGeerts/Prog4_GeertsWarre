#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject *go, const std::vector<EventId> &listenEventIds,
                                    const EventId sendEventId)
    : Component(go, "ScoreComponent"), m_ListenEventIds{listenEventIds}, m_SendEventId{sendEventId} {
    for (const auto &eventId: m_ListenEventIds) {
        m_Handles.emplace_back(EventManager::GetInstance().AttachEvent(
            eventId,
            [this](const Event &event) { IncreaseScore(std::get<int>(event.args[0])); }
        ));
    }
}

dae::ScoreComponent::~ScoreComponent() {
    if (!EventManager::GetInstance().isAlive()) return;
    for (const auto &handle: m_Handles) {
        EventManager::GetInstance().DetachEvent(handle);
    }
}

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

nlohmann::ordered_json dae::ScoreComponent::Serialize() const {
    nlohmann::ordered_json data;
    data["handles"] = nlohmann::json::array();
    data["event_ids"] = nlohmann::json::array();
    for (auto [eventId, index, valid]: m_Handles) {
        data["handles"].push_back({
            {"event_id", eventId},
            {"index", index},
            {"valid", valid}
        });
    }
    for (auto id: m_ListenEventIds) {
        data["event_ids"].push_back(id);
    }
    return data;
}

void dae::ScoreComponent::Deserialize(const nlohmann::ordered_json &data) {
    m_Handles.clear();
    m_ListenEventIds.clear();

    if (data.contains("handles") && data["handles"].is_array()) {
        for (const auto &handleData: data["handles"]) {
            int eventId = handleData.value("event_id", 0);
            int index = handleData.value("index", 0);
            bool valid = handleData.value("valid", false);
            m_Handles.emplace_back(eventId, index, valid);
        }
    }

    if (data.contains("event_ids") && data["event_ids"].is_array()) {
        for (const auto &idData: data["event_ids"]) {
            m_ListenEventIds.push_back(idData.get<int>());
        }
    }
}

void dae::ScoreComponent::IncreaseScore(const int amount) {
    m_Score += amount;
    EventManager::GetInstance().SendEvent(Event(m_SendEventId).AddInt(m_Score));
}
