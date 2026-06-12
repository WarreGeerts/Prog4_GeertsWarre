#include "HamburgerManager.h"
#include "GameObject.h"
#include "SDL3/SDL_log.h"
#include "States/GSManager.h"

namespace game {
    void HamburgerManager::Update() {
        if (IsLevelComplete()) {
            if (!m_Finished) {
                m_Finished = true;
                SDL_Log("Level Clear");
                GSManager::GetInstance().NextLevel();
            }
        }
    }

    void HamburgerManager::InspectorGUI() {
        ImGui::InputInt("Parts Required per Plate", &m_RequiredPartsPerPlate);
        if (m_RequiredPartsPerPlate < 1) m_RequiredPartsPerPlate = 1;

        ImGui::Separator();

        ImGui::Text("Registered Plates (%zu):", m_Plates.size());

        for (size_t i = 0; i < m_Plates.size(); ++i) {
            if (m_Plates[i] == nullptr) continue;

            std::string name = m_Plates[i]->GetGameObject()->GetName();

            bool isFull = m_Plates[i]->IsFull(m_RequiredPartsPerPlate);

            ImGui::BulletText("%s: %d %s",
                              name.c_str(),
                              m_RequiredPartsPerPlate,
                              isFull ? "[FULL]" : "");
        }
    }

    nlohmann::ordered_json HamburgerManager::Serialize() const {
        nlohmann::ordered_json data;
        data["requiredParts"] = m_RequiredPartsPerPlate;
        return data;
    }

    void HamburgerManager::Deserialize(const nlohmann::ordered_json &data) {
        m_RequiredPartsPerPlate = data.value("requiredParts", 1);
    }

    void HamburgerManager::RegisterPlate(HamburgerHolderComponent *plate) {
        m_Plates.push_back(plate);
    }

    bool HamburgerManager::IsLevelComplete() const {
        if (m_Plates.empty()) return false;

        for (const auto *plate: m_Plates) {
            if (!plate->IsFull(m_RequiredPartsPerPlate)) return false;
        }
        return true;
    }
}
