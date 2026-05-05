#include "LivesDisplayComponent.h"
#include "GameObject.h"
using namespace dae;

LivesDisplayComponent::LivesDisplayComponent(GameObject *go, const EventId eventId)
    : Component(go, "LivesDisplayComponent"), m_ListenEventId(eventId) {
    SetHandle(m_ListenEventId);
}

LivesDisplayComponent::~LivesDisplayComponent() {
    if (!EventManager::GetInstance().isAlive()) return;
    EventManager::GetInstance().DetachEvent(m_Handle);
}

void LivesDisplayComponent::SetHandle(const EventId eventId) {
    if (m_Handle.valid) {
        EventManager::GetInstance().DetachEvent(m_Handle);
    }

    m_Handle = EventManager::GetInstance().AttachEvent(
        eventId,
        [this](const Event &event) {
            OnPlayerDied(std::get<int>(event.args[0]));
        });
}

void LivesDisplayComponent::Update() {
    if (!m_IsActive) return;

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

#if defined(_WIN32) || defined(_WIN64)
    strncpy_s(textBuffer, sizeof(textBuffer), m_Text.c_str(), _TRUNCATE);
#else
    strncpy(textBuffer, m_Text.c_str(), sizeof(textBuffer) - 1);
    textBuffer[sizeof(textBuffer) - 1] = '\0';
#endif

    if (ImGui::InputText("Display Text##LD", textBuffer, sizeof(textBuffer))) {
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

nlohmann::ordered_json LivesDisplayComponent::Serialize() const {
    nlohmann::ordered_json data;
    data["max_lives"] = m_MaxLives;
    data["event_id"] = m_ListenEventId;
    data["handle"] = {
        {"event_id", m_Handle.eventId},
        {"index", m_Handle.index},
        {"valid", m_Handle.valid},
    };
    data["text"] = m_Text;
    return data;
}

void LivesDisplayComponent::Deserialize(const nlohmann::ordered_json &data) {
    m_MaxLives = data.value("max_lives", 3);
    m_ListenEventId = data.value("event_id", -1);
    if (data.contains("handle")) {
        m_Handle.eventId = data.value("event_id", -1);
        m_Handle.index = data.value("index", -1);
        m_Handle.valid = data.value("valid", false);
    }
    m_Text = data.value("text", "# Lives: ");
}
