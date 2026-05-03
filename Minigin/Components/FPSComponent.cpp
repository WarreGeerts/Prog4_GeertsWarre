#include "FPSComponent.h"
#include <cassert>
#include "Singletons/DeltaTime.h"
#include <iomanip>
#include "GameObject.h"
using namespace dae;

FPSComponent::FPSComponent(GameObject *go)
    : Component(go, "FPSComponent") {}

void FPSComponent::Update() {
    if (!m_IsActive) return;

    if (!onLoad) {
        m_TextComponentRef = m_gameObject->GetComponent<TextComponent>();
        onLoad = true;
    }

    m_Frames++;
    m_TotalTime += DeltaTime::GetInstance().Time();
    m_ElapsedTime += DeltaTime::GetInstance().Time();

    if (m_ElapsedTime >= 1.f) {
        m_FPS = static_cast<float>(m_Frames) / m_ElapsedTime;
        m_ElapsedTime = 0.f;
        m_Frames = 0;
    }
}

void FPSComponent::Render() const {
    if (!m_IsActive) return;

    const float currentFPS{m_FPS};
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << currentFPS;

    if (m_TextComponentRef)
        m_TextComponentRef->SetText("FPS: " + ss.str());
}

void FPSComponent::InspectorGUI() {
    ImGui::Text("FPS Debug Values");
    ImGui::InputFloat("FPS", &m_FPS, 0, 0, nullptr, ImGuiInputTextFlags_ReadOnly);
    ImGui::InputFloat("Elapsed Time", &m_ElapsedTime, 0, 0, nullptr, ImGuiInputTextFlags_ReadOnly);
    ImGui::InputInt("Frames", &m_Frames, 1, 100, ImGuiInputTextFlags_ReadOnly);
    ImGui::InputFloat("Total Time", &m_TotalTime, 0, 0, nullptr, ImGuiInputTextFlags_ReadOnly);
}

nlohmann::ordered_json FPSComponent::Serialize() const {
    nlohmann::ordered_json data;
    return data;
}

void FPSComponent::Deserialize(const nlohmann::ordered_json &data) {}
