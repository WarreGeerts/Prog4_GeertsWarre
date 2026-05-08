#include "UiGameModeSelector.h"
#include "GameObject.h"
#include "Components/TextComponent.h"

namespace game {
    UiGameModeSelector::UiGameModeSelector(ge::GameObject *go)
        : Component(go, "UiGameModeSelector"), m_CurrentOption(0) {
        AddHandles();
    }

    void UiGameModeSelector::Update() {
        if (!m_LateLoad) {
            m_gameObject->GetChildAt(0)->GetComponent<ge::TextComponent>()->SetColor(m_ColorSelected);
            m_LateLoad = true;
        }
    }

    void UiGameModeSelector::InspectorGUI() {
        //Color
        float defaultColor[3] = {
            static_cast<float>(m_ColorDefault.r) / 255.0f,
            static_cast<float>(m_ColorDefault.g) / 255.0f,
            static_cast<float>(m_ColorDefault.b) / 255.0f
        };

        if (ImGui::ColorEdit3("Default Text Color##UIgms", defaultColor)) {
            m_ColorDefault.r = static_cast<Uint8>(defaultColor[0] * 255.0f);
            m_ColorDefault.g = static_cast<Uint8>(defaultColor[1] * 255.0f);
            m_ColorDefault.b = static_cast<Uint8>(defaultColor[2] * 255.0f);
        }

        //Color
        float selectedColor[3] = {
            static_cast<float>(m_ColorSelected.r) / 255.0f,
            static_cast<float>(m_ColorSelected.g) / 255.0f,
            static_cast<float>(m_ColorSelected.b) / 255.0f
        };

        if (ImGui::ColorEdit3("Selected Text Color##UIgms", selectedColor)) {
            m_ColorSelected.r = static_cast<Uint8>(selectedColor[0] * 255.0f);
            m_ColorSelected.g = static_cast<Uint8>(selectedColor[1] * 255.0f);
            m_ColorSelected.b = static_cast<Uint8>(selectedColor[2] * 255.0f);
        }
    }

    nlohmann::ordered_json UiGameModeSelector::Serialize() const {
        return nlohmann::ordered_json();
    }

    void UiGameModeSelector::Deserialize(const nlohmann::ordered_json &) {
        AddHandles();
    }

    void UiGameModeSelector::AddHandles() {
        if (m_HandleDown.valid) {
            ge::EventManager::GetInstance().DetachEvent(m_HandleDown);
        }
        m_HandleDown = ge::EventManager::GetInstance().AttachEvent(
            ge::EventRegistry::UI_SELECTION_DOWN,
            [this](const ge::Event &) {
                this->SelectionDown();
            });

        if (m_HandleUp.valid) {
            ge::EventManager::GetInstance().DetachEvent(m_HandleUp);
        }
        m_HandleUp = ge::EventManager::GetInstance().AttachEvent(
            ge::EventRegistry::UI_SELECTION_UP,
            [this](const ge::Event &) {
                this->SelectionUp();
            });
    }

    void UiGameModeSelector::SwitchSelection() {
        m_gameObject->GetChildAt(m_PrevOption)->GetComponent<ge::TextComponent>()->SetColor(m_ColorDefault);

        if (m_CurrentOption < 0) m_CurrentOption = m_gameObject->GetChildCount() - 1;
        m_CurrentOption %= m_gameObject->GetChildCount();

        m_gameObject->GetChildAt(m_CurrentOption)->GetComponent<ge::TextComponent>()->SetColor(m_ColorSelected);
        m_PrevOption = m_CurrentOption;
    }

    void UiGameModeSelector::SelectionDown() {
        --m_CurrentOption;
        SwitchSelection();
    }

    void UiGameModeSelector::SelectionUp() {
        ++m_CurrentOption;
        SwitchSelection();
    }
}
