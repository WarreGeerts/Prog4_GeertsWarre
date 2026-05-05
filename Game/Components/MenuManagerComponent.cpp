#include "MenuManagerComponent.h"

#include "GameObject.h"
#include "Singletons/DeltaTime.h"

namespace dae {
    void MenuManagerComponent::SwitchInfo() {
        m_gameObject->GetChildAt(m_CurrentChildIndex)->SetActive(false);

        ++m_CurrentChildIndex;
        m_CurrentChildIndex %= m_gameObject->GetChildCount();

        m_gameObject->GetChildAt(m_CurrentChildIndex)->SetActive(true);
    }

    void MenuManagerComponent::Update() {
        m_AccTime += DeltaTime::GetInstance().Time();

        if (m_AccTime >= m_WaitTime) {
            m_AccTime = 0.0f;
            SwitchInfo();
        }
    }

    void MenuManagerComponent::InspectorGUI() {
        float waitTime[1] = {m_WaitTime};

        if (ImGui::InputFloat("Wait Time", waitTime)) {
            m_WaitTime = waitTime[0];
        }
    }

    nlohmann::ordered_json MenuManagerComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["wait_time"] = m_WaitTime;
        return data;
    }
    void MenuManagerComponent::Deserialize(const nlohmann::ordered_json &data) {
        m_WaitTime = data.value("wait_time", 3.0f);
    }
}
