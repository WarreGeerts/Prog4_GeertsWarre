#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class ScoreComponent final : public Component {
    public:
        explicit ScoreComponent(GameObject *go) : Component(go, "ScoreComponent"){}

        explicit ScoreComponent(GameObject *go, const std::vector<EventId> &listenEventIds, const EventId sendEventId)
            : Component(go,"ScoreComponent"), m_ListenEventIds{listenEventIds}, m_SendEventId{sendEventId} {
            for (const auto &eventId: m_ListenEventIds) {
                m_Handles.emplace_back(EventManager::GetInstance().AttachEvent(
                    eventId,
                    [this](const Event &event) { IncreaseScore(std::get<int>(event.args[0])); }
                ));
            }
        }

        ~ScoreComponent() override {
            for (const auto &handle: m_Handles) {
                EventManager::GetInstance().DetachEvent(handle);
            }
        }


        void Update() override {};
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override {
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

        void Deserialize(const nlohmann::ordered_json &data) override {
            m_Handles.clear();
            m_ListenEventIds.clear();

            if (data.contains("handles") && data["handles"].is_array()) {
                for (const auto& handleData : data["handles"]) {
                    int eventId = handleData.value("event_id", 0);
                    int index = handleData.value("index", 0);
                    bool valid = handleData.value("valid", false);
                    m_Handles.emplace_back(eventId, index, valid);
                }
            }

            if (data.contains("event_ids") && data["event_ids"].is_array()) {
                for (const auto& idData : data["event_ids"]) {
                    m_ListenEventIds.push_back(idData.get<int>());
                }
            }
        }

        void IncreaseScore(const int amount) {
            m_Score += amount;
            EventManager::GetInstance().SendEvent(Event(m_SendEventId).AddInt(m_Score));
        }

    private:
        std::vector<EventId> m_ListenEventIds{};
        EventId m_SendEventId{};
        std::vector<EventHandle> m_Handles;
        int m_Score{};
    };
}
