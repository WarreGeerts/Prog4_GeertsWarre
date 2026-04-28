#include "EditorGui.h"
#include "Components/Components.h"
//ImGui
#include "imgui.h"
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
//scene
#include "Singletons/SceneManager.h"
using namespace dae;
static GameObject *selectedGO = nullptr;

void EditorGui::RenderGUI() {
    //scene graph
    DrawSceneGraph();

    //Inspector
    DrawInspector(selectedGO);
}

//Main function to draw the sceneGraph tab
void EditorGui::DrawSceneGraph() {
    ImGui::Begin("Scene Graph");
    for (auto &getScene: SceneManager::GetInstance().GetScenes()) {
        ImGui::Text(getScene->GetName().c_str());
        for (auto &GO: getScene->GetGameObjects()) {
            if (GO->GetParent() == nullptr)
                VisualizeSceneGraph(GO.get());
        }
    }
    ImGui::End();
}

//Recursive function made to visualize the SceneGraph objects per scene
void EditorGui::VisualizeSceneGraph(GameObject *GO) {
    if (!GO) return;
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (GO->GetChildCount() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (selectedGO == GO) flags |= ImGuiTreeNodeFlags_Selected;

    const bool opened = ImGui::TreeNodeEx((void *) GO, flags, "%s", GO->GetName().c_str());

    if (ImGui::IsItemClicked()) {
        selectedGO = GO;
    }

    if (opened) {
        for (int idx = 0; idx < GO->GetChildCount(); ++idx) {
            VisualizeSceneGraph(GO->GetChildAt(idx));
        }
        ImGui::TreePop();
    }
}

//Main function to draw the inspector tab
void EditorGui::DrawInspector(GameObject *GO) {
    ImGui::Begin("Inspector");

    if (!GO) {
        ImGui::Text("Select an object to see properties.");
        ImGui::End();
        return;
    }

    char nameBuffer[128]; //Max size of name that user can put inside
    strcpy_s(nameBuffer, sizeof(nameBuffer), GO->GetName().c_str());
    if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
        GO->SetName(nameBuffer);
    }
    ImGui::Separator();

    //Every gameObject has this by default and shall never get deleted!
    GO->GetTransform()->InspectorGUI();

    Component *componentToPendingRemoval = nullptr;

    for (auto &Comp: GO->GetComponents()) {
        if (dynamic_cast<TransformComponent *>(Comp.get())) continue;

        const bool hasWarning = Comp->HasWarning();

        if (hasWarning) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.f, 0.f, 0.95f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        const bool open = ImGui::CollapsingHeader(Comp->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen);

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Remove Component")) {
                componentToPendingRemoval = Comp.get();
                ImGui::EndPopup();
                break;
            }
            ImGui::EndPopup();
        }

        if (open) {
            Comp->InspectorGUI();
            ImGui::Text(" ");
        }

        if (hasWarning) {
            ImGui::PopStyleColor(2);
        }
    }

    if (componentToPendingRemoval) {
        GO->RemoveComponent(componentToPendingRemoval);
    }

    if (ImGui::Button("Add Component...", ImVec2(-1, 0))) {
        ImGui::OpenPopup("AddComponentPopup");
    }

    DrawAddComponentPopup(GO);

    ImGui::End();
}

void EditorGui::DrawAddComponentPopup(GameObject *GO) {
    if (ImGui::BeginPopup("AddComponentPopup")) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Add Component");
        ImGui::Separator();
        if (ImGui::BeginMenu("Rendering")) {
            DrawAddComponentItem<RenderComponent>(GO, "Render Component");
            DrawAddComponentItem<SpriteComponent>(GO, "Sprite Component");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("UI")) {
            DrawAddComponentItem<TextComponent>(GO, "Text Component");
            DrawAddComponentItem<FPSComponent>(GO, "FPS Component");
            DrawAddComponentItem<LivesDisplayComponent>(GO, "Lives Display Component");
            DrawAddComponentItem<ScoreDisplayComponent>(GO, "Score Display Component");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Controlling")) {
            DrawAddComponentItem<CharacterControllerComponent>(GO, "Character Controller Component");
            DrawAddComponentItem<RotateComponent>(GO, "Rotate Component");
            DrawAddComponentItem<LivesComponent>(GO, "Lives Component");
            DrawAddComponentItem<ScoreComponent>(GO, "Score Component");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("UnListed")) {
            DrawAddComponentItem<ThrashCacheComponent>(GO, "Thrash Cache Component");
            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
}

template<typename T>
void EditorGui::DrawAddComponentItem(GameObject *GO, const char *label) {
    const bool hasComp = GO->HasComponent<T>();

    if (ImGui::MenuItem(label, nullptr, false, !hasComp)) {
        GO->AddComponent(std::make_unique<T>(GO));
        ImGui::CloseCurrentPopup();
    }

    if (hasComp && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Object already has this component.");
    }
}
