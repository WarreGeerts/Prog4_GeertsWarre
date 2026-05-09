#include "CollisionGridComponent.h"

namespace game {
    CollisionGridComponent::CollisionGridComponent(ge::GameObject *go) : Component(go, "CollisionGridComponent") {
        ResizeGrid();
    }
    void CollisionGridComponent::Update() {}
    void CollisionGridComponent::Render() const {}

    void CollisionGridComponent::InspectorGUI() {
        if (ImGui::InputInt("Columns", &m_Columns)) { ResizeGrid(); }
        if (ImGui::InputInt("Rows", &m_Rows)) { ResizeGrid(); }

        ImGui::Separator();

        static int current_brush = 0;
        ImGui::Text("Brush:");
        if (ImGui::RadioButton("Empty", current_brush == 0)) { current_brush = 0; } ImGui::SameLine();
        if (ImGui::RadioButton("Ground", current_brush == 1)) { current_brush = 1; } ImGui::SameLine();
        if (ImGui::RadioButton("Ladder", current_brush == 2)) { current_brush = 2; }

        ImGui::Spacing();
        ImGui::Text("Click cells to paint:");


        ImVec2 button_sz(25, 25);

        ImGui::BeginGroup();
        for (int y = 0; y < m_Rows; y++) {
            for (int x = 0; x < m_Columns; x++) {
                int idx = GetIndex(x, y);

                ImGui::PushID(idx);

                ImVec4 color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
                const char* label = "";

                if (m_GridData[idx] == 1) { color = ImVec4(0.2f, 0.8f, 0.2f, 1.0f); label = "G"; }
                if (m_GridData[idx] == 2) { color = ImVec4(0.8f, 0.6f, 0.2f, 1.0f); label = "L"; }

                ImGui::PushStyleColor(ImGuiCol_Button, color);

                if (ImGui::Button(label, button_sz)) {
                    m_GridData[idx] = current_brush;
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
