#include "ScoreComponent.h"

namespace game {
    ScoreComponent::ScoreComponent(ge::GameObject *go, const std::vector<ge::EventId> &listenEventIds,
                                   const ge::EventId sendEventId)
        : Component(go, "ScoreComponent"), m_ListenEventIds{listenEventIds}, m_SendEventId{sendEventId} {
        for (const auto &eventId: m_ListenEventIds) {
            m_Handles.emplace_back(ge::EventManager::GetInstance().AttachEvent(
                eventId,
                [this](const ge::Event &event) { IncreaseScore(std::get<int>(event.args[0])); }
            ));
        }
    }

    ScoreComponent::~ScoreComponent() {
        if (!ge::EventManager::GetInstance().isAlive()) return;
        for (const auto &handle: m_Handles) {
            ge::EventManager::GetInstance().DetachEvent(handle);
        }
    }

    void ScoreComponent::InspectorGUI() {
        auto &em = ge::EventManager::GetInstance();

        if (ImGui::InputInt("Current Score", &m_Score, 1, 10)) {
            em.SendEvent(ge::Event(m_SendEventId).AddInt(m_Score));
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

        static ge::EventId idToAdd = 0;
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
                m_Handles.push_back(em.AttachEvent(idToAdd, [this](const ge::Event &e) {
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

    nlohmann::ordered_json ScoreComponent::Serialize() const {
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
        data["broadcast_events"] = m_SendEventId;

        return data;
    }

    void ScoreComponent::Deserialize(const nlohmann::ordered_json &data) {
        m_Handles.clear();
        m_ListenEventIds.clear();

        if (data.contains("event_ids") && data["event_ids"].is_array()) {
            for (const auto &idData : data["event_ids"]) {
                ge::EventId id = idData.get<int>();
                m_ListenEventIds.push_back(id);

                m_Handles.emplace_back(ge::EventManager::GetInstance().AttachEvent(
                    id,
                    [this](const ge::Event &event) { IncreaseScore(std::get<int>(event.args[0])); }
                ));
            }
        }
        m_SendEventId = data.value("broadcast_events", -1);
    }

    void ScoreComponent::IncreaseScore(const int amount) {
        m_Score += amount;
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_SendEventId).AddInt(m_Score));
    }
}
