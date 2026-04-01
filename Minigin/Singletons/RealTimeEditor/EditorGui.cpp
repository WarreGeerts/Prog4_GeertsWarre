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

    char nameBuffer[64]; //Max size of name that user can put inside
    strcpy(nameBuffer, GO->GetName().c_str());
    if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
        GO->SetName(nameBuffer);
    }
    ImGui::Separator();

    //Every gameObject has this by default and shall never get deleted!
    GO->GetTransform()->InspectorGUI();

    for (auto &Comp: GO->GetComponents()) {
        if (dynamic_cast<TransformComponent *>(Comp.get())) continue;

        const bool open = ImGui::CollapsingHeader(Comp->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen);

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Remove Component")) {
                GO->RemoveComponent(Comp.get());
                ImGui::EndPopup();
                return;
            }
            ImGui::EndPopup();
        }

        if (open) {
            Comp->InspectorGUI();
        }
    }

    if (ImGui::Button("Add Component...", ImVec2(-1, 0))) {
        ImGui::OpenPopup("AddComponentPopup");
    }

    //DrawAddComponentPopup(GO);

    ImGui::End();
}

void EditorGui::DrawTransformComponent(GameObject *GO) {
    float pos[2] = {};
    if (ImGui::DragFloat2("Position", pos, 0.1f)) {
        GO->SetLocalPosition(glm::vec3(pos[0], pos[1], 0));
    }
}
