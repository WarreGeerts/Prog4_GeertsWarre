#include "CollisionGridComponent.h"
#include "GameObject.h"
#include "Singletons/Renderer.h"

namespace game {
    CollisionGridComponent::CollisionGridComponent(ge::GameObject *go) : Component(go, "CollisionGridComponent") {
        ResizeGrid();
    }

    void CollisionGridComponent::Update() {}

    void CollisionGridComponent::Render() const {
        SDL_Renderer *renderer = ge::Renderer::GetInstance().GetSDLRenderer();

        const float worldOffsetX = m_gameObject->GetTransform()->GetPosition().x;
        const float worldOffsetY = m_gameObject->GetTransform()->GetPosition().y;

        for (int y = 0; y < m_Rows; y++) {
            for (int x = 0; x < m_Columns; x++) {
                const int idx = GetIndex(x, y);
                if (m_GridData[idx] == 0) continue;

                SDL_FRect rect;
                rect.x = worldOffsetX + (x * 16 * m_Scale);
                rect.y = worldOffsetY + (y * 16 * m_Scale);
                rect.w = 16 * m_Scale;
                rect.h = 16 * m_Scale;

                if (m_GridData[idx] == 1) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
                } else if (m_GridData[idx] == 2) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 100);
                } else if (m_GridData[idx] == 3) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 100);
                }

                SDL_RenderFillRect(renderer, &rect);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderRect(renderer, &rect);
            }
        }
    }

    void CollisionGridComponent::InspectorGUI() {
        ImGui::Checkbox("Show Debug Colliders", &m_DebugLines);

        if (ImGui::InputInt("Columns", &m_Columns)) { ResizeGrid(); }
        if (ImGui::InputInt("Rows", &m_Rows)) { ResizeGrid(); }
        if (ImGui::InputFloat("Scale", &m_Scale)) { ResizeGrid(); }

        ImGui::Separator();

        ImGui::Text("Brush:");
        if (ImGui::RadioButton("Empty", m_CurrentBrush == 0)) { m_CurrentBrush = 0; }
        ImGui::SameLine();
        if (ImGui::RadioButton("Ground", m_CurrentBrush == 1)) { m_CurrentBrush = 1; }
        ImGui::SameLine();
        if (ImGui::RadioButton("Ladder", m_CurrentBrush == 2)) { m_CurrentBrush = 2; }
        ImGui::SameLine();
        if (ImGui::RadioButton("Ladder (down)", m_CurrentBrush == 3)) { m_CurrentBrush = 3; }

        ImGui::Spacing();
        ImGui::Text("Click cells to paint:");

        constexpr ImVec2 button_sz(25, 25);

        ImGui::BeginGroup();
        for (int y = 0; y < m_Rows; y++) {
            for (int x = 0; x < m_Columns; x++) {
                int idx = GetIndex(x, y);

                ImGui::PushID(idx);

                auto color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
                auto label = "";

                if (m_GridData[idx] == 1) {
                    color = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
                    label = "G";
                }
                if (m_GridData[idx] == 2) {
                    color = ImVec4(0.8f, 0.6f, 0.2f, 1.0f);
                    label = "L";
                }
                if (m_GridData[idx] == 3) {
                    color = ImVec4(0.2f, 0.6f, 0.8f, 1.0f);
                    label = "Ld";
                }

                ImGui::PushStyleColor(ImGuiCol_Button, color);

                if (ImGui::Button(label, button_sz)) {
                    m_GridData[idx] = m_CurrentBrush;
                }

                if (x < m_Columns - 1) ImGui::SameLine();

                ImGui::PopStyleColor();
                ImGui::PopID();
            }
        }
        ImGui::EndGroup();
    }

    nlohmann::ordered_json CollisionGridComponent::Serialize() const {
        return {};
    }

    void CollisionGridComponent::Deserialize(const nlohmann::ordered_json &) {}

    int CollisionGridComponent::GetIndex(const int x, const int y) const {
        return y * m_Columns + x;
    }

    void CollisionGridComponent::ResizeGrid() {
        m_GridData.resize(m_Columns * m_Rows, 0);
    }
}
