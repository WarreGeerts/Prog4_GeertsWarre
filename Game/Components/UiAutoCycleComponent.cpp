#include "UiAutoCycleComponent.h"
#include "GameObject.h"
#include "Singletons/DeltaTime.h"

namespace game {
    UiAutoCycleComponent::UiAutoCycleComponent(ge::GameObject *go, const float waitTime)
        : Component(go, "UiAutoCycleComponent"), m_WaitTime(waitTime) {
    }

    void UiAutoCycleComponent::SwitchInfo() {
        m_gameObject->GetChildAt(m_CurrentChildIndex)->SetActive(false);

        ++m_CurrentChildIndex;
        m_CurrentChildIndex %= m_gameObject->GetChildCount();

        m_gameObject->GetChildAt(m_CurrentChildIndex)->SetActive(true);
    }

    void UiAutoCycleComponent::Update() {
        m_AccTime += ge::DeltaTime::GetInstance().Time();

        if (m_AccTime >= m_WaitTime) {
            m_AccTime = 0.0f;
            SwitchInfo();
        }
    }

    void UiAutoCycleComponent::InspectorGUI() {
        float waitTime[1] = {m_WaitTime};

        if (ImGui::InputFloat("Wait Time", waitTime)) {
            m_WaitTime = waitTime[0];
        }
    }

    nlohmann::ordered_json UiAutoCycleComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["wait_time"] = m_WaitTime;
        data["current_child_index"] = m_CurrentChildIndex;
        return data;
    }

    void UiAutoCycleComponent::Deserialize(const nlohmann::ordered_json &data) {
        m_WaitTime = data.value("wait_time", 3.0f);
        m_CurrentChildIndex = data.value("current_child_index", 0);
    }
}
