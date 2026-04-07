#include "LivesDisplayComponent.h"
#include "GameObject.h"
using namespace dae;

void LivesDisplayComponent::Update() {
    if (NeedsUpdate()) {
        m_TextComponentRef = m_gameObject->GetComponent<TextComponent>();
    }

    if (m_TextComponentRef && (m_Lives != m_PrevLives)) {
        m_TextComponentRef->SetText(m_Text + std::to_string(m_Lives));
        m_PrevLives = m_Lives;
    }
}


void LivesDisplayComponent::InspectorGUI() {
    m_HasWarning = false;
    if (!m_TextComponentRef) {
        m_HasWarning = true;
        if (ImGui::IsItemHovered()) {
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.f, 0.f, 0.95f));

            ImGui::BeginTooltip();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "WARNING: TextComponent is NEEDED");
            ImGui::EndTooltip();

            ImGui::PopStyleColor();
        }
    }

    //Text
    char textBuffer[256];
    strncpy(textBuffer, m_Text.c_str(), sizeof(textBuffer));
    textBuffer[sizeof(textBuffer) - 1] = '\0';

    if (ImGui::InputText("Display Text", textBuffer, sizeof(textBuffer))) {
        m_Text = textBuffer;
    }

    ImGui::InputInt("Lives", &m_Lives, 0, 0, ImGuiInputTextFlags_ReadOnly);

    //EventId dropdown box
    const auto &em = EventManager::GetInstance();

    const std::string currentName = em.GetEventName(m_ListenEventId);
    const char *comboPreview = m_ListenEventId == 0 ? "None Selected" : currentName.c_str();

    if (ImGui::BeginCombo("Listen Event ID's##LDC", comboPreview)) {
        for (const auto &[id, name]: em.GetRegisteredEvents()) {
            const bool isSelected = (m_ListenEventId == id);

            if (ImGui::Selectable(name.c_str(), isSelected)) {
                m_ListenEventId = id;
                SetHandle(m_ListenEventId);
            }

            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}
