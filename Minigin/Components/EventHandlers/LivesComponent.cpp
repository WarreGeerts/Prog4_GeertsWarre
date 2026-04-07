#include "LivesComponent.h"

void dae::LivesComponent::InspectorGUI() {
    ImGui::InputInt("Lives", &m_Lives, 0, 0);
    EventManager::GetInstance().SendEvent(Event(m_SendEventId).AddInt(m_Lives));
}
