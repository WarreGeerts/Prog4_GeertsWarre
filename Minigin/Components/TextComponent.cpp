#include "TextComponent.h"
#include "Component.h"
#include "Components/EventHandlers/LivesDisplayComponent.h"
#include "Components/EventHandlers/ScoreDisplayComponent.h"
#include <filesystem>
#include "GameObject.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "Font.h"
#include "Texture2D.h"
#include <utility>
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
using namespace dae;
//default constructor
TextComponent::TextComponent(GameObject *go)
    : Component(go, "TextComponent"), m_needsUpdate(true), m_FileName("Lingua.otf"), m_text("New Text"),
      m_textTexture(nullptr) {
    m_font = ResourceManager::GetInstance().LoadFont(m_FileName, static_cast<uint8_t>(m_fontSize));
}

//extra one go constructor
TextComponent::TextComponent(GameObject *go, std::string text, std::string fileName, const int fontSize,
                             const SDL_Color &color)
    : Component(go, "TextComponent"), m_needsUpdate(true), m_FileName(std::move(fileName)), m_text(std::move(text)),
      m_color(color),
      m_fontSize(fontSize), m_textTexture(nullptr) {
    m_font = ResourceManager::GetInstance().LoadFont(m_FileName, static_cast<uint8_t>(m_fontSize));
}

void TextComponent::Update() {
    if (NeedsUpdate()) {
        m_renderComponentRef = m_gameObject->GetComponent<RenderComponent>();

        //check if components that make use of the Text component/overwrite the usage exist
        m_OverWriten = m_gameObject->HasComponent<LivesDisplayComponent>() ||
                       m_gameObject->HasComponent<ScoreDisplayComponent>();
    }

    if (!m_renderComponentRef) return;

    if (m_text != m_prevText) {
        m_needsUpdate = true;
    }

    if ((m_needsUpdate || m_GuiUpdated) && m_font != nullptr) {
        const char *textToRender = m_text.empty() ? " " : m_text.c_str();

        const auto surf = TTF_RenderText_Blended(m_font->GetFont(), textToRender, m_text.length(), m_color);
        if (surf == nullptr) {
            return;
        }
        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        SDL_DestroySurface(surf);

        m_textTexture = std::make_shared<Texture2D>(texture);
        m_needsUpdate = false;
        m_GuiUpdated = false;
        m_prevText = m_text;
    }

    if (m_GuiUpdated) {
        ResourceManager::GetInstance().UnloadUnusedFonts();
    }
}

void TextComponent::Render() const {
    if (m_renderComponentRef && m_textTexture) {
        m_renderComponentRef->SetTexture(m_textTexture);
        m_renderComponentRef->Render();
    }
}

void TextComponent::InspectorGUI() {
    m_HasWarning = false;
    if (!m_renderComponentRef) {
        m_HasWarning = true;
        if (ImGui::IsItemHovered()) {
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.f, 0.f, 0.95f));

            ImGui::BeginTooltip();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "WARNING: RenderComponent is NEEDED");
            ImGui::EndTooltip();

            ImGui::PopStyleColor();
        }
    }

    const std::string path{"./Data/"};

    std::vector<std::string> availableTextures{RenderComponent::GetTextureFiles(path, true)};

    const char *currentLabel{m_FileName.empty() ? "None Selected" : m_FileName.c_str()};

    //Text
    if (m_OverWriten) {
        ImGui::BeginDisabled();
    }

    char textBuffer[256];

#if defined(_WIN32) || defined(_WIN64)
    // Windows: Use the secure version
    strncpy_s(textBuffer, sizeof(textBuffer), m_text.c_str(), _TRUNCATE);
#else
    strncpy(textBuffer, m_text.c_str(), sizeof(textBuffer) - 1);
    textBuffer[sizeof(textBuffer) - 1] = '\0';
#endif

    if (ImGui::InputText("Display Text##TC", textBuffer, sizeof(textBuffer))) {
        m_text = textBuffer;
        m_GuiUpdated = true;
    }

    if (m_OverWriten) {
        ImGui::EndDisabled();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "(?)");
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Text is managed by other Component.");
    }

    //Font Size
    int fontSize[1] = {m_fontSize};
    if (ImGui::InputInt("Font Size##TC", fontSize)) {
        m_fontSize = fontSize[0];
        m_font = ResourceManager::GetInstance().LoadFont(m_FileName, static_cast<uint8_t>(m_fontSize));
        m_GuiUpdated = true;
    }

    //Color
    float color[3] = {
        static_cast<float>(m_color.r) / 255.0f,
        static_cast<float>(m_color.g) / 255.0f,
        static_cast<float>(m_color.b) / 255.0f
    };

    if (ImGui::ColorEdit3("Text Color##TC", color)) {
        m_color.r = static_cast<Uint8>(color[0] * 255.0f);
        m_color.g = static_cast<Uint8>(color[1] * 255.0f);
        m_color.b = static_cast<Uint8>(color[2] * 255.0f);

        m_font = ResourceManager::GetInstance().LoadFont(m_FileName, static_cast<uint8_t>(m_fontSize));
        m_GuiUpdated = true;
    }

    //File selection
    if (ImGui::BeginCombo("Fonts##TC", currentLabel)) {
        for (const auto &file: availableTextures) {
            const bool isSelected = (m_FileName == file);

            if (ImGui::Selectable(file.c_str(), isSelected)) {
                m_FileName = file;
                m_font = ResourceManager::GetInstance().LoadFont(m_FileName, static_cast<uint8_t>(m_fontSize));
                m_GuiUpdated = true;
            }

            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Reset Textures##TC")) {
        availableTextures = RenderComponent::GetTextureFiles(path, true);
    }

#if !defined(_WIN32) && !defined(_WIN64)
    ImGui::BeginDisabled();
#endif

    if (ImGui::Button("Open Folder##TC")) {
#if defined(_WIN32) || defined(_WIN64)
        std::filesystem::path absPath = std::filesystem::absolute(path);
        const std::string windowsPath = absPath.make_preferred().string();
        ShellExecuteA(nullptr, "open", windowsPath.c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
#endif

#if !defined(_WIN32) && !defined(_WIN64)
        ImGui::EndDisabled();
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("Opening folder is only supported on Windows.");
        }
#endif
    }
}
