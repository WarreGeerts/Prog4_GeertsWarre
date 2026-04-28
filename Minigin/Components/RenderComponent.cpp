#include "RenderComponent.h"
#include "GameObject.h"
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <filesystem>
#include "TextComponent.h"
#include "Texture2D.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
using namespace dae;

RenderComponent::RenderComponent(GameObject *go)
    : Component(go, "RenderComponent") {}

RenderComponent::RenderComponent(GameObject *go, const std::string &filename)
    : RenderComponent(go) {
    SetTexture(filename);
    m_FileName = filename;
}

void RenderComponent::Update() {
    if (NeedsUpdate()) {
        //check if components that make use of the render component/overwrite the usage exist
        m_OverWriten = m_gameObject->HasComponent<SpriteComponent>() ||
                       m_gameObject->HasComponent<TextComponent>();

        m_transform = m_gameObject->GetTransform().get();
        m_sprite = m_gameObject->GetComponent<SpriteComponent>();
    }
}

void RenderComponent::Render() const {
    float posX{0};
    float posY{0};

    if (m_transform) {
        posX = m_transform->GetPosition().x;
        posY = m_transform->GetPosition().y;
    }

    if (m_sprite) {
        auto pTexture = m_sprite->GetTexture();
        if (pTexture && pTexture->GetSDLTexture()) {
            const SDL_FRect src{
                m_sprite->GetSrcRect().x, m_sprite->GetSrcRect().y,
                m_sprite->GetSrcRect().w, m_sprite->GetSrcRect().h
            };
            const SDL_FRect dst{
                posX + m_sprite->GetDstRect().x, posY + m_sprite->GetDstRect().y,
                m_sprite->GetDstRect().w, m_sprite->GetDstRect().h
            };
            Renderer::GetInstance().RenderTextureRegion(*pTexture, &src, &dst);
        }
    } else {
        if (m_texture && m_texture->GetSDLTexture()) {
            Renderer::GetInstance().RenderTexture(*m_texture, posX, posY);
        }
    }
}

void RenderComponent::InspectorGUI() {
    const std::string path{"./Data/"};

    std::vector<std::string> availableTextures{GetTextureFiles(path)};

    const char *currentLabel{m_FileName.empty() ? "None Selected" : m_FileName.c_str()};

    if (m_OverWriten) {
        ImGui::BeginDisabled();
    }
    if (ImGui::BeginCombo("Texture##RC", currentLabel)) {
        for (const auto &file: availableTextures) {
            const bool isSelected = (m_FileName == file);

            if (ImGui::Selectable(file.c_str(), isSelected)) {
                m_FileName = file;
                SetTexture(m_FileName);
            }

            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Reset Textures##RC")) {
        availableTextures = GetTextureFiles(path);
    }

    if (m_OverWriten) {
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "(?)");
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Texture is managed by other Component.");
    }

#if !defined(_WIN32) && !defined(_WIN64)
    ImGui::BeginDisabled();
#endif

    if (ImGui::Button("Open Folder##RC")) {
#if defined(_WIN32) || defined(_WIN64)
        std::filesystem::path absPath = std::filesystem::absolute(path);
        const std::string windowsPath = absPath.make_preferred().string();
        ShellExecuteA(nullptr, "open", windowsPath.c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
#endif
    }

#if !defined(_WIN32) && !defined(_WIN64)
    ImGui::EndDisabled();
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Opening folder is only supported on Windows.");
    }
#endif
}

void RenderComponent::SetTexture(const std::string &filename) {
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

std::vector<std::string> RenderComponent::GetTextureFiles(const std::string &directory, bool font) {
    std::vector<std::string> files;
    if (!std::filesystem::exists(directory)) return files;

    for (const auto &entry: std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            auto ext = entry.path().extension().string();
            if (!font) {
                if (ext == ".png" || ext == ".jpg") {
                    files.push_back(entry.path().filename().string());
                }
            } else {
                if (ext == ".otf") {
                    files.push_back(entry.path().filename().string());
                }
            }
        }
    }
    return files;
}
