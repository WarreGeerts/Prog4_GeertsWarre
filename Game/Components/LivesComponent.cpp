#include "LivesComponent.h"

#include "GameObject.h"
#include "SDL3/SDL_log.h"
#include "Singletons/DeltaTime.h"
#include "Sound/SdlSoundSystem.h"
#include "Sound/ServiceLocator.h"

namespace game {
    LivesComponent::LivesComponent(ge::GameObject *go, const std::vector<ge::EventId> &listenEventIds,
                                   const ge::EventId sendEventId)
        : ge::Component(go, "LivesComponent"), m_ListenEventIds(listenEventIds), m_SendEventId{sendEventId} {

        for (const auto &eventId: m_ListenEventIds) {
            m_Handles.emplace_back(ge::EventManager::GetInstance().AttachEvent(
                eventId,
                [this](const ge::Event &) {
                    PlayerDie();
                }
            ));
        }
    }

    LivesComponent::LivesComponent(ge::GameObject *go, const std::vector<ge::EventId> &listenEventIds,
                                   const ge::EventId sendEventId, const int lives)
        : ge::Component(go, "LivesComponent"), m_ListenEventIds(listenEventIds), m_SendEventId{sendEventId},
          m_Lives{lives} {
        for (const auto &eventId: m_ListenEventIds) {
            m_Handles.emplace_back(ge::EventManager::GetInstance().AttachEvent(
                eventId,
                [this](const ge::Event &) {
                    PlayerDie();
                }
            ));
        }
    }

    LivesComponent::~LivesComponent() {
        if (!ge::EventManager::GetInstance().isAlive()) return;
        for (const auto &handle: m_Handles) {
            ge::EventManager::GetInstance().DetachEvent(handle);
        }
    }

    void LivesComponent::Update() {
        if (m_Damaged) {
            m_Invincibility = true;

            m_AccTime += ge::DeltaTime::GetInstance().Time();

            if (m_AccTime >= m_IMaxTime) {
                m_Invincibility = false;
                m_Damaged = false;
                m_AccTime = 0;
            }
        }

        if (m_Lives < 0) {
            m_GameOver = true;
        }
    }

    void LivesComponent::InspectorGUI() {
        auto &em = ge::EventManager::GetInstance();

        ImGui::Checkbox("Invincible##LC", &m_Invincible);

        if (ImGui::InputInt("Lives", &m_Lives, 1, 1)) {
            em.SendEvent(ge::Event(m_SendEventId).AddInt(m_Lives));
        }

        ImGui::Text("Triggers (Listening to):");
        if (ImGui::BeginListBox("##ListenList", ImVec2(-FLT_MIN, 150))) {
            for (int i = 0; i < static_cast<int>(m_ListenEventIds.size()); ++i) {
                std::string name = em.GetEventName(m_ListenEventIds[i]);

                ImGui::PushID(i);

                if (ImGui::Selectable(name.c_str(), false)) {}

                if (ImGui::BeginPopupContextItem("ItemContext")) {
                    if (ImGui::MenuItem("Delete Listener")) {
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
                m_Handles.push_back(em.AttachEvent(idToAdd, [this](const ge::Event &) {
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

    nlohmann::ordered_json LivesComponent::Serialize() const {
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

    void LivesComponent::Deserialize(const nlohmann::ordered_json &data) {
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
        m_SendEventId = data.value("broadcast_events", -1);

        for (const auto& id : m_ListenEventIds) {
            m_Handles.emplace_back(ge::EventManager::GetInstance().AttachEvent(
                id, [this](const ge::Event&) { PlayerDie(); }));
        }
    }

    void LivesComponent::PlayerDie() {
        if (m_Invincibility || m_Invincible) return;
        m_Damaged = true;
        --m_Lives;
        auto &ss = ge::ServiceLocator::GetSoundSystem();
        ss.Play(3, 0.5f);
        ge::EventManager::GetInstance().SendEvent(ge::Event(m_SendEventId).AddInt(m_Lives));
    }
}
