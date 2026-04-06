#include "LivesDisplayComponent.h"
#include <cassert>
using namespace dae;

void LivesDisplayComponent::Render() const {
    assert(m_TextComponentRef != nullptr);
    m_TextComponentRef->SetText(m_Text + std::to_string(m_Lives));
}

void LivesDisplayComponent::InspectorGUI() {
    //Text
    char textBuffer[256];
    strncpy(textBuffer, m_Text.c_str(),sizeof(textBuffer));
    textBuffer[sizeof(textBuffer)-1] = '\0';

    if (ImGui::InputText("Display Text", textBuffer, sizeof(textBuffer))) {
        m_Text = textBuffer;
    }

    ImGui::InputInt("Lives", &m_Lives,0,0,ImGuiInputTextFlags_ReadOnly);
}
