#include "CollisionGridComponent.h"
#include "GameObject.h"
#include "Singletons/Renderer.h"

namespace game {
    CollisionGridComponent::CollisionGridComponent(ge::GameObject *go) : Component(go, "CollisionGridComponent") {
        ResizeGrid();
    }

    void CollisionGridComponent::Update() {}

    void CollisionGridComponent::Render() const {
        if (!m_DebugLines) return;
        SDL_Renderer *renderer{ge::Renderer::GetInstance().GetSDLRenderer()};
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        const float worldOffsetX{m_gameObject->GetTransform()->GetPosition().x};
        const float worldOffsetY{m_gameObject->GetTransform()->GetPosition().y};

        for (int y = 0; y < m_Rows; y++) {
            for (int x = 0; x < m_Columns; x++) {
                const int idx{GetIndex(x, y)};
                if (m_GridData[idx] == 0) continue;

                SDL_FRect rect;
                rect.x = worldOffsetX + (static_cast<float>(x) * static_cast<float>(m_CurrentTileSize) * m_Scale);
                rect.y = worldOffsetY + (static_cast<float>(y) * static_cast<float>(m_CurrentTileSize) * m_Scale);
                rect.w = static_cast<float>(m_CurrentTileSize) * m_Scale;
                rect.h = static_cast<float>(m_CurrentTileSize) * m_Scale;

                if (m_GridData[idx] == 1) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
                } else if (m_GridData[idx] == 2) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 100);
                } else if (m_GridData[idx] == 3) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 100);
                } else if (m_GridData[idx] == 4) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
                }

                SDL_RenderFillRect(renderer, &rect);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                SDL_RenderRect(renderer, &rect);
            }
        }
    }

    void CollisionGridComponent::InspectorGUI() {
        ImGui::Checkbox("Show Debug Colliders", &m_DebugLines);

        if (ImGui::InputInt("Columns", &m_Columns)) { ResizeGrid(); }
        if (ImGui::InputInt("Rows", &m_Rows)) { ResizeGrid(); }
        ImGui::InputInt("Tile Size", &m_CurrentTileSize);

        ImGui::SliderInt("Brush Size", &m_BrushSize, 1, 10);
        ImGui::SliderFloat("Editor Zoom", &m_VisualScale, 2.0f, 30.0f);

        ImGui::SeparatorText("Brushes");
        const char *brushNames[]{"Empty", "Ground", "Ladder", "Ladder (Down)", "Intersection"};
        constexpr int amountBrushes{5};
        for (int i = 0; i < amountBrushes; i++) {
            if (ImGui::RadioButton(brushNames[i], m_CurrentBrush == i)) { m_CurrentBrush = i; }
            if (i < amountBrushes - 1) ImGui::SameLine();
        }

        ImGui::Spacing();

        ImGui::Text("Canvas (Drag to paint, Right-click to erase):");

        ImGui::BeginChild("GridRegion", ImVec2(300, 300), true,
                          ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);
        const ImVec2 canvasSz(static_cast<float>(m_Columns) * m_VisualScale,
                              static_cast<float>(m_Rows) * m_VisualScale);

        const ImVec2 canvasP0{ImGui::GetCursorScreenPos()};

        const auto canvasP1{ImVec2(canvasP0.x + canvasSz.x, canvasP0.y + canvasSz.y)};

        ImDrawList *drawList{ImGui::GetWindowDrawList()};

        drawList->AddRectFilled(canvasP0, canvasP1, IM_COL32(30, 30, 30, 255));

        ImGui::InvisibleButton("canvas_interaction", canvasSz);

        bool isHovered{ImGui::IsItemHovered()};
        if (isHovered) {
            const ImVec2 mousePos{ImGui::GetIO().MousePos};
            const int mouseX{static_cast<int>((mousePos.x - canvasP0.x) / m_VisualScale)};
            const int mouseY{static_cast<int>((mousePos.y - canvasP0.y) / m_VisualScale)};

            if (ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                const int brushToUse{ImGui::IsMouseDown(ImGuiMouseButton_Left) ? m_CurrentBrush : 0};

                for (int dy = 0; dy < m_BrushSize; dy++) {
                    for (int dx = 0; dx < m_BrushSize; dx++) {
                        const int tx{mouseX + dx};
                        const int ty{mouseY + dy};
                        if (tx >= 0 && tx < m_Columns && ty >= 0 && ty < m_Rows) {
                            m_GridData[GetIndex(tx, ty)] = brushToUse;
                        }
                    }
                }
            }
        }

        for (int y = 0; y < m_Rows; y++) {
            for (int x = 0; x < m_Columns; x++) {
                const int val = m_GridData[GetIndex(x, y)];
                if (val == 0) continue;

                ImU32 cellCol;
                if (val == 1) cellCol = IM_COL32(50, 200, 50, 255);
                else if (val == 2) cellCol = IM_COL32(200, 150, 50, 255);
                else if (val == 3) cellCol = IM_COL32(50, 150, 200, 255);
                else cellCol = IM_COL32(50, 0, 200, 255);

                auto cellP0{
                    ImVec2(canvasP0.x + static_cast<float>(x) * m_VisualScale,
                           canvasP0.y + static_cast<float>(y) * m_VisualScale)
                };
                auto cellP1{ImVec2(cellP0.x + m_VisualScale, cellP0.y + m_VisualScale)};

                drawList->AddRectFilled(cellP0, cellP1, cellCol);
            }
        }

        if (m_VisualScale > 5.0f) {
            for (int i = 0; i <= m_Columns; i++)
                drawList->AddLine(ImVec2(canvasP0.x + static_cast<float>(i) * m_VisualScale, canvasP0.y),
                                  ImVec2(canvasP0.x + static_cast<float>(i) * m_VisualScale, canvasP1.y),
                                  IM_COL32(100, 100, 100, 40));
            for (int i = 0; i <= m_Rows; i++)
                drawList->AddLine(ImVec2(canvasP0.x, canvasP0.y + static_cast<float>(i) * m_VisualScale),
                                  ImVec2(canvasP1.x, canvasP0.y + static_cast<float>(i) * m_VisualScale),
                                  IM_COL32(100, 100, 100, 40));
        }

        ImGui::EndChild();
    }

    nlohmann::ordered_json CollisionGridComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["columns"] = m_Columns;
        data["rows"] = m_Rows;
        data["scale"] = m_Scale;
        data["tile_size"] = m_CurrentTileSize;

        //Make sure the grid doesn't take too much space inside the JSON file,
        //currently using RLE (Run-Length Encoding) and Sparse mapping (hybrid of the 2)
        //stores current position of the tile + how many tiles after each other are the same + what type of tile it is
        //so you would get something like this:
        //"grid_tile_index": "[number_repeating_tiles,tile_type]" | ("1006": "[3,2]")
        nlohmann::json rle_data = nlohmann::json::object();
        for (int i = 0; i < static_cast<int>(m_GridData.size());) {
            if (m_GridData[i] == 0) {
                ++i;
                continue;
            }

            const int startIdx{i};
            const int val{m_GridData[i]};
            int count{0};

            while (i < static_cast<int>(m_GridData.size()) && m_GridData[i] == val) {
                ++count;
                ++i;
            }

            //Better than multi line, brought a level file size from 81kb down to 18kb
            //so a very good improvement
            rle_data[std::to_string(startIdx)] = "[" + std::to_string(count) + "," + std::to_string(val) + "]";
        }

        data["grid_rle"] = rle_data;

        return data;
    }

    void CollisionGridComponent::Deserialize(const nlohmann::ordered_json &data) {
        m_Columns = data.value("columns", 1);
        m_Rows = data.value("rows", 1);
        m_Scale = data.value("scale", 1.0f);
        m_CurrentTileSize = data.value("tile_size", 16);

        m_GridData.assign(m_Columns * m_Rows, 0);

        if (data.contains("grid_rle")) {
            for (auto &[key, value]: data["grid_rle"].items()) {
                const int startIdx{std::stoi(key)};
                auto rawStr{value.get<std::string>()};

                int count{0};
                int tileType{0};
                char discardBracket;
                char discardComma;
                std::stringstream ss(rawStr);
                if (ss >> discardBracket >> count >> discardComma >> tileType) {
                    for (int i = 0; i < count; ++i) {
                        const int targetIdx{startIdx + i};
                        if (targetIdx < static_cast<int>(m_GridData.size())) {
                            m_GridData[targetIdx] = tileType;
                        }
                    }
                }
            }
        }
    }

    std::vector<int> &CollisionGridComponent::GetMapCollisionGrid() {
        return m_GridData;
    }

    int CollisionGridComponent::GetTileTypeAtWorldPosition(const float worldX, const float worldY) const {
        const float localX{worldX - m_gameObject->GetTransform()->GetPosition().x};
        const float localY{worldY - m_gameObject->GetTransform()->GetPosition().y};

        const float finalTileSize{static_cast<float>(m_CurrentTileSize) * m_Scale};
        const int x{static_cast<int>(std::floor(localX / finalTileSize))};
        const int y{static_cast<int>(std::floor(localY / finalTileSize))};

        if (x >= 0 && x < m_Columns && y >= 0 && y < m_Rows) {
            return m_GridData[GetIndex(x, y)];
        }
        return 0;
    }

    int CollisionGridComponent::GetIndex(const int x, const int y) const {
        return y * m_Columns + x;
    }

    void CollisionGridComponent::ResizeGrid() {
        m_GridData.resize(m_Columns * m_Rows, 0);
    }
}
