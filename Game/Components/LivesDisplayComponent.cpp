#include "LivesDisplayComponent.h"
#include "GameObject.h"
#include "SDL3/SDL_log.h"

namespace game {
    LivesDisplayComponent::LivesDisplayComponent(ge::GameObject *go, const ge::EventId eventId)
        : Component(go, "LivesDisplayComponent"), m_ListenEventId(eventId) {
        SetHandle(m_ListenEventId);
    }

    LivesDisplayComponent::~LivesDisplayComponent() {
        if (!ge::EventManager::GetInstance().isAlive()) return;
        ge::EventManager::GetInstance().DetachEvent(m_Handle);
    }

    void LivesDisplayComponent::SetHandle(const ge::EventId eventId) {
        if (m_Handle.valid) {
            ge::EventManager::GetInstance().DetachEvent(m_Handle);
        }

        m_Handle = ge::EventManager::GetInstance().AttachEvent(
            eventId,
            [this](const ge::Event &event) {
                OnPlayerDied(std::get<int>(event.args[0]));
            });
    }

    void LivesDisplayComponent::Update() {
        if (!m_Handle.valid) {
            SetHandle(m_ListenEventId);
        }

        if (!m_IsActive) return;

        if (!m_TextComponentRef) {
            m_TextComponentRef = m_gameObject->GetComponent<ge::TextComponent>();
        }

        if (m_DisplayMode == DisplayMode::Sprites && m_LifeSprites.empty()) {
            m_LifeSprites.clear();
            for (int i = 0; i < m_gameObject->GetChildCount(); ++i) {
                m_LifeSprites.push_back(m_gameObject->GetChildAt(i));
            }
        }

        if (m_DisplayMode == DisplayMode::Text) {
            for (auto *sprite: m_LifeSprites) {
                if (sprite->IsActive()) sprite->SetActive(false);
            }

            if (m_TextComponentRef && (m_Lives != m_PrevLives)) {
                m_TextComponentRef->SetText(m_Text + std::to_string(m_Lives));
                m_PrevLives = m_Lives;
            }
        } else if (m_DisplayMode == DisplayMode::Sprites) {
            if (m_TextComponentRef && m_TextComponentRef->GetActive()) {
                m_TextComponentRef->ChangeActive(false);
            }

            for (size_t i = 0; i < m_LifeSprites.size(); ++i) {
                const int displayLives = (m_Lives < 0) ? 0 : m_Lives;

                bool shouldBeVisible = (i < static_cast<size_t>(displayLives));

                if (m_LifeSprites[i]->IsActive() != shouldBeVisible) {
                    m_LifeSprites[i]->SetActive(shouldBeVisible);
                }
            }
        }
    }

    void LivesDisplayComponent::InspectorGUI() {
        const char *modes[] = {"Text", "Sprites"};
        int modeIndex = static_cast<int>(m_DisplayMode);
        if (ImGui::Combo("Display Mode", &modeIndex, modes, IM_ARRAYSIZE(modes))) {
            m_DisplayMode = static_cast<DisplayMode>(modeIndex);
        }

        if (m_DisplayMode == DisplayMode::Text) {
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
        } else {
            if (ImGui::Button("Refresh Sprite Cache")) {
                if (m_DisplayMode == DisplayMode::Sprites && m_LifeSprites.empty()) {
                    m_LifeSprites.clear();
                    for (int i = 0; i < m_gameObject->GetChildCount(); ++i) {
                        m_LifeSprites.push_back(m_gameObject->GetChildAt(i));
                    }
                }
            }

            ImGui::Text("Using children as sprites.");
            ImGui::Text("Ensure child order matches life count.");
        }

        ImGui::InputInt("Lives", &m_Lives, 0, 0, ImGuiInputTextFlags_ReadOnly);

        //EventId dropdown box
        const auto &em = ge::EventManager::GetInstance();

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
        data["display_mode"] = static_cast<int>(m_DisplayMode);
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
        m_DisplayMode = static_cast<DisplayMode>(data.value("display_mode", 0));
        SetHandle(m_ListenEventId);
    }

    void LivesDisplayComponent::OnPlayerDied(const int lives) {
        m_Lives = lives;
    }
}
