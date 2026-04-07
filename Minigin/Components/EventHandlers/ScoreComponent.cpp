#include "ScoreComponent.h"

void dae::ScoreComponent::InspectorGUI() {
    ImGui::InputInt("Score", &m_Score, 0, 0);
    EventManager::GetInstance().SendEvent(Event(m_SendEventId).AddInt(m_Score));
}
