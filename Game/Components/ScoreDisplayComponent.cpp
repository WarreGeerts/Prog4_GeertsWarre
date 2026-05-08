#include "ScoreDisplayComponent.h"
#include "GameObject.h"
namespace game {
    ScoreDisplayComponent::ScoreDisplayComponent(ge::GameObject *go, const ge::EventId eventId)
        : Component(go, "ScoreDisplayComponent"),
          m_ListenEventId(eventId) {
        SetHandle(m_ListenEventId);
    }

    ScoreDisplayComponent::~ScoreDisplayComponent() {
        if (!ge::EventManager::GetInstance().isAlive()) return;
        ge::EventManager::GetInstance().DetachEvent(m_Handle);
    }

    void ScoreDisplayComponent::SetHandle(const ge::EventId eventId) {
        if (m_Handle.valid) {
            ge::EventManager::GetInstance().DetachEvent(m_Handle);
        }

        m_Handle = ge::EventManager::GetInstance().AttachEvent(
            eventId,
            [this](const ge::Event &event) {
                OnScoreIncrease(std::get<int>(event.args[0]));
            });
    }

    void ScoreDisplayComponent::Update() {
        if (!m_IsActive) return;

        if (NeedsUpdate()) {
            m_TextComponentRef = m_gameObject->GetComponent<ge::TextComponent>();
        }

        if (m_TextComponentRef && (m_Score != m_PrevScore)) {
            m_TextComponentRef->SetText(m_Text + std::to_string(m_Score));
            m_PrevScore = m_Score;
        }
    }

    void ScoreDisplayComponent::InspectorGUI() {
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

        if (ImGui::InputText("Display Text##SD", textBuffer, sizeof(textBuffer))) {
            m_Text = textBuffer;
        }

        //score display
        ImGui::InputInt("Score", &m_Score, 0, 0, ImGuiInputTextFlags_ReadOnly);

        //EventId dropdown box
        const auto &em = ge::EventManager::GetInstance();

        const std::string currentName = em.GetEventName(m_ListenEventId);
        const char *comboPreview = m_ListenEventId == 0 ? "None Selected" : currentName.c_str();

        if (ImGui::BeginCombo("Listen Event ID's##SDC", comboPreview)) {
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

    nlohmann::ordered_json ScoreDisplayComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["event_id"] = m_ListenEventId;
        data["handle"] = {
            {"event_id", m_Handle.eventId},
            {"index", m_Handle.index},
            {"valid", m_Handle.valid},
        };
        data["text"] = m_Text;
        return data;
    }

    void ScoreDisplayComponent::Deserialize(const nlohmann::ordered_json &data) {
        m_ListenEventId = data.value("event_id", -1);
        if (data.contains("handle")) {
            m_Handle.eventId = data.value("event_id", -1);
            m_Handle.index = data.value("index", -1);
            m_Handle.valid = data.value("valid", false);
        }
        m_Text = data.value("text", "# Lives: ");
    }
}