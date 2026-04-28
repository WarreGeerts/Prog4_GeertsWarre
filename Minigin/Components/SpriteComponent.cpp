#include "SpriteComponent.h"
#include <algorithm>
#include <vector>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#include "Singletons/ResourceManager.h"
#include "Texture2D.h"
#include <SDL3/SDL_render.h>
#include "GameObject.h"
#include "RenderComponent.h"
using namespace dae;

SpriteComponent::SpriteComponent(GameObject *go, const std::string &spritesheetPath, const SpriteFraming &spriteFrame,
                                 const int frameIdx)
    : Component(go, "SpriteComponent"), m_FileName(spritesheetPath), m_SpriteFrame(spriteFrame),
      m_FrameIndex(frameIdx) {
    SetSpritesheetPath(spritesheetPath);
    m_DstRect = {0.f, 0.f, spriteFrame.frameWidth, spriteFrame.frameHeight};
    SetFrame(frameIdx);
}

void SpriteComponent::SetSpritesheetPath(const std::string &spritesheetPath) {
    if (spritesheetPath.empty()) return;

    auto texture = ResourceManager::GetInstance().LoadTexture(spritesheetPath);
    if (texture && texture->GetSDLTexture()) {
        m_Spritesheet = texture;
        const auto size{m_Spritesheet->GetSize()};

        if (m_SpriteFrame.frameWidth <= 0) m_SpriteFrame.frameWidth = size.x;
        if (m_SpriteFrame.frameHeight <= 0) m_SpriteFrame.frameHeight = size.y;

        //m_SrcRect = {0.f, 0.f, size.x, size.y};
        SDL_SetTextureScaleMode(m_Spritesheet->GetSDLTexture(), SDL_SCALEMODE_PIXELART);

        SetFrame(m_FrameIndex);
    }
}

void SpriteComponent::SetFrame(const int) {
    if (m_SpriteFrame.columns <= 0 || m_SpriteFrame.rows <= 0) return;

    const int totalFrames = m_SpriteFrame.columns * m_SpriteFrame.rows;
    m_FrameIndex = (m_FrameIndex % totalFrames + totalFrames) % totalFrames;

    const int col{m_FrameIndex % m_SpriteFrame.columns};
    const int row{m_FrameIndex / m_SpriteFrame.columns};

    m_SrcRect.x = static_cast<float>(col) * (m_SpriteFrame.frameWidth + m_SpriteFrame.spacingX);
    m_SrcRect.y = static_cast<float>(row) * (m_SpriteFrame.frameHeight + m_SpriteFrame.spacingY);
    m_SrcRect.w = m_SpriteFrame.frameWidth;
    m_SrcRect.h = m_SpriteFrame.frameHeight;

    m_DstRect.w = m_SpriteFrame.frameWidth * m_ScaleX;
    m_DstRect.h = m_SpriteFrame.frameHeight * m_ScaleY;
}

void SpriteComponent::InspectorGUI() {
    if (NeedsUpdate()) {
        if (m_gameObject->HasComponent<RenderComponent>()) m_HasRenderComponent = true;
        else m_HasRenderComponent = false;
    }

    m_HasWarning = false;
    if (!m_HasRenderComponent) {
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

    std::vector<std::string> availableTextures{RenderComponent::GetTextureFiles(path)};

    const char *currentLabel{m_FileName.empty() ? "None Selected" : m_FileName.c_str()};

    //Frame index selection
    int frameIdx[1] = {m_FrameIndex};

    if (ImGui::InputInt("Frame Index", frameIdx)) {
        m_FrameIndex = frameIdx[0];
        SetFrame(m_FrameIndex);
    }

    //Amount cols and rows
    int columns_rows[2] = {m_SpriteFrame.columns, m_SpriteFrame.rows};
    if (ImGui::InputInt2("Columns & Rows", columns_rows)) {
        m_SpriteFrame.columns = columns_rows[0];
        m_SpriteFrame.rows = columns_rows[1];
        SetFrame(m_FrameIndex);
    }

    //Frame width and height
    float width_height[2] = {m_SpriteFrame.frameWidth, m_SpriteFrame.frameHeight};
    if (ImGui::DragFloat2("Frame Width & Height", width_height, 0.01f)) {
        m_SpriteFrame.frameWidth = width_height[0];
        m_SpriteFrame.frameHeight = width_height[1];
        SetFrame(m_FrameIndex);
    }

    //Frame Spacing
    float SpacingX_Y[2] = {m_SpriteFrame.spacingX, m_SpriteFrame.spacingY};
    if (ImGui::DragFloat2("Frame Spacing X & Y", SpacingX_Y, 0.01f)) {
        m_SpriteFrame.spacingX = SpacingX_Y[0];
        m_SpriteFrame.spacingY = SpacingX_Y[1];
        SetFrame(m_FrameIndex);
    }

    //Frame scale
    float ScaleX_Y[2] = {m_ScaleX, m_ScaleY};
    if (ImGui::DragFloat2("Scale X & Y", ScaleX_Y, 0.01f)) {
        m_ScaleX = ScaleX_Y[0];
        m_ScaleY = ScaleX_Y[1];
        SetFrame(m_FrameIndex);
    }

    //File selection
    if (ImGui::BeginCombo("Sprite Sheet##SC", currentLabel)) {
        for (const auto &file: availableTextures) {
            const bool isSelected = (m_FileName == file);

            if (ImGui::Selectable(file.c_str(), isSelected)) {
                m_FileName = file;
                SetSpritesheetPath(m_FileName);
                SetFrame(m_FrameIndex);
            }

            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Reset Textures##SC")) {
        availableTextures = RenderComponent::GetTextureFiles(path);
    }

#if !defined(_WIN32) && !defined(_WIN64)
    ImGui::BeginDisabled();
#endif

    ImGui::SameLine();
    if (ImGui::Button("Open Folder##SC")) {
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

void SpriteComponent::SetScale(const float scaleX, const float scaleY) {
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
    m_DstRect.w = m_SpriteFrame.frameWidth * m_ScaleX;
    m_DstRect.h = m_SpriteFrame.frameHeight * m_ScaleY;
}
