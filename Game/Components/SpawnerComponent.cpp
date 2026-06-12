#include "SpawnerComponent.h"
#include "EnemyComponent.h"
#include "Components/ColliderComponent.h"
#include "Singletons/SceneManager.h"

namespace game {
    void SpawnerComponent::Update() {
        if ((!m_Start && m_Deserialized)) {
            if (!m_LinkedGOName.empty()) {
                for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                         ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
                    if (obj->GetName() == m_LinkedGOName) {
                        m_linkedGo = obj.get();
                        m_Respawn = true;
                        break;
                    }
                }
            }
            m_Start = true;
        }

        if (m_linkedGo != nullptr) {
            auto* enemyComp = m_linkedGo->GetComponent<EnemyComponent>();
            if (enemyComp && enemyComp->Dead()) {
                m_Respawn = true;
            }

            if (m_Respawn) {
                m_linkedGo->SetLocalPosition({m_SpawnPosition.x, m_SpawnPosition.y, 0});
                m_Respawn = false;
            }
        }
    }

    void SpawnerComponent::InspectorGUI() {
        if (ImGui::Button("Respawn##SC")) {
            m_Respawn = true;
        }

        float spawnPosition[2] = {m_SpawnPosition.x, m_SpawnPosition.y};

        if (ImGui::InputFloat2("Position", spawnPosition)) {
            m_SpawnPosition.x = spawnPosition[0];
            m_SpawnPosition.y = spawnPosition[1];
        }

        char buf[128];
#if defined(_WIN32) || defined(_WIN64)
        strncpy_s(buf, sizeof(buf), m_LinkedGOName.c_str(), _TRUNCATE);
#else
        strncpy(buf, m_LinkedGOName.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
#endif

        if (ImGui::InputText("Linked GameObject##SC", buf, sizeof(buf))) {
            m_LinkedGOName = buf;
        }
    }

    nlohmann::ordered_json SpawnerComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["linkedName"] = m_LinkedGOName;
        data["spawnPosition"] = {m_SpawnPosition.x, m_SpawnPosition.y};
        return data;
    }

    void SpawnerComponent::Deserialize(const nlohmann::ordered_json &data) {
        if (data.contains("linkedName") && !data["linkedName"].is_null()) {
            m_LinkedGOName = data["linkedName"].get<std::string>();
        }

        if (data.contains("spawnPosition") && data["spawnPosition"].is_array()) {
            auto &posArray = data["spawnPosition"];

            if (posArray.size() >= 2) {
                m_SpawnPosition.x = posArray[0].get<float>();
                m_SpawnPosition.y = posArray[1].get<float>();
            }
        }
        m_Deserialized = true;
    }
}
