#include "TextComponent.h"
#include "Component.h"
#include "TransformComponent.h"
#include "Components/EventHandlers/LivesDisplayComponent.h"
#include "Components/EventHandlers/ScoreDisplayComponent.h"
#include <filesystem>
#include "GameObject.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "Font.h"
#include "Texture2D.h"
#include <stdexcept>
#include <windows.h>
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
using namespace dae;
//default constructor
TextComponent::TextComponent(GameObject *go)
    : Component(go, "TextComponent"), m_needsUpdate(false), m_text(" "), m_font(nullptr), m_textTexture(nullptr) {}

//extra one go constructor
TextComponent::TextComponent(GameObject *go, const std::string &text, const std::string &fileName, int fontSize,
                             const SDL_Color &color)
    : Component(go, "TextComponent"), m_needsUpdate(true), m_FileName(fileName), m_text(text), m_color(color),
      m_fontSize(fontSize), m_textTexture(nullptr) {
    m_font = ResourceManager::GetInstance().LoadFont(m_FileName, m_fontSize);
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
    } else {
        m_needsUpdate = false;
    }

    if (m_needsUpdate || m_GuiUpdated) {
        const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
        if (surf == nullptr) {
            throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
        }
        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        if (texture == nullptr) {
            throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
        }
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
    if (!m_renderComponentRef) return;
    m_renderComponentRef->SetTexture(m_textTexture);
    m_renderComponentRef->Render();
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
    strncpy(textBuffer, m_text.c_str(), sizeof(textBuffer));
    textBuffer[sizeof(textBuffer) - 1] = '\0';

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
        m_font = ResourceManager::GetInstance().LoadFont(m_FileName, m_fontSize);
        m_GuiUpdated = true;
    }

    //Color
    int color[3] = {m_color.r, m_color.g, m_color.b};
    if (ImGui::DragInt3("[RGB] Color##TC", color, 1, 0, 255)) {
        m_color.r = color[0];
        m_color.g = color[1];
        m_color.b = color[2];
        m_font = ResourceManager::GetInstance().LoadFont(m_FileName, m_fontSize);
        m_GuiUpdated = true;
    }

    //File selection
    if (ImGui::BeginCombo("Fonts##TC", currentLabel)) {
        for (const auto &file: availableTextures) {
            const bool isSelected = (m_FileName == file);

            if (ImGui::Selectable(file.c_str(), isSelected)) {
                m_FileName = file;
                m_font = ResourceManager::GetInstance().LoadFont(m_FileName, m_fontSize);
                m_GuiUpdated = true;
            }

            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Reset Textures##TC")) {
        availableTextures = RenderComponent::GetTextureFiles(path, true);
    }

    ImGui::SameLine();
    if (ImGui::Button("Open Folder##TC")) {
        std::filesystem::path absPath = std::filesystem::absolute(path);
        const std::string windowsPath = absPath.make_preferred().string();
        ShellExecuteA(NULL, "open", windowsPath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
    }
}
