#include "ScoreDisplayComponent.h"
#include "GameObject.h"
using namespace dae;

void ScoreDisplayComponent::Update() {
    if (NeedsUpdate()) {
        m_TextComponentRef = m_gameObject->GetComponent<TextComponent>();
    }
}

void ScoreDisplayComponent::Render() const {
    if (m_TextComponentRef)
        m_TextComponentRef->SetText(m_Text + std::to_string(m_Score));
}

void ScoreDisplayComponent::InspectorGUI() {
    //Text
    char textBuffer[256];
    strncpy(textBuffer, m_Text.c_str(), sizeof(textBuffer));
    textBuffer[sizeof(textBuffer) - 1] = '\0';

    if (ImGui::InputText("Display Text", textBuffer, sizeof(textBuffer))) {
        m_Text = textBuffer;
    }

    ImGui::InputInt("Score", &m_Score, 0, 0, ImGuiInputTextFlags_ReadOnly);
}
