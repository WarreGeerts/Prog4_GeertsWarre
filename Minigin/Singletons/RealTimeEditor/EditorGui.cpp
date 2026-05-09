#include "EditorGui.h"
#include "Components/EngineComponents.h"
//ImGui
#include "imgui.h"
//scene
#include <SDL3/SDL_log.h>
#include "ComponentFactory.h"
#include "SceneSerializer.h"
#include "Input/InputManager.h"
#include "Singletons/SceneManager.h"

namespace ge {
    static GameObject *selectedGO = nullptr;

    EditorGui::EditorGui() {
        m_FileBrowser.SetTitle("Select a Scene");
        m_FileBrowser.SetTypeFilters({".json"});
    }

    void EditorGui::RenderGUI() {
        //top-bar
        DrawTopBar();

        //scene graph
        if (m_ShowSceneGraph)
            DrawSceneGraph();

        //Inspector
        if (m_ShowInspector)
            DrawInspector(selectedGO);
    }

    //Scene Graph
#pragma region SceneGraph
    //TODO: bug, selection change when making new name will change new selected one as well
    void EditorGui::DrawSceneGraph() {
        ImGui::Begin("Scene Graph");

        if (ImGui::BeginPopupContextWindow("SceneGraphContext",
                                           ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
            if (ImGui::MenuItem("Create Empty GameObject")) {
                auto &scene = SceneManager::GetInstance().GetSceneByIdx(
                    SceneManager::GetInstance().GetCurrentSceneIdx());
                scene.Add(std::make_unique<GameObject>("Empty GameObject"));
            }
            ImGui::EndPopup();
        }

        auto &currentScene = SceneManager::GetInstance().
                GetSceneByIdx(SceneManager::GetInstance().GetCurrentSceneIdx());
        const std::string currentSceneName = currentScene.GetName();
        char sceneNameBuf[64] = {};
        strncpy(sceneNameBuf, currentSceneName.c_str(), sizeof(sceneNameBuf) - 1);

        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);

        if (ImGui::InputText("##SceneNameEdit", sceneNameBuf, sizeof(sceneNameBuf))) {
            currentScene.SetName(sceneNameBuf);
        }

        ImGui::PopItemWidth();

        for (auto &GO: currentScene.GetGameObjects()) {
            if (GO->GetParent() == nullptr)
                VisualizeSceneGraph(GO.get());
        }

        ImGui::Dummy(ImGui::GetContentRegionAvail());
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_HIERARCHY")) {
                GameObject *draggedGO = *static_cast<GameObject **>(payload->Data);
                draggedGO->SetParent(nullptr, true);
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::End();
    }

    void EditorGui::VisualizeSceneGraph(GameObject *GO) {
        if (!GO || GO->MarkedForDeletion()) return;
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (GO->GetChildCount() == 0) flags |= ImGuiTreeNodeFlags_Leaf;
        if (selectedGO == GO) flags |= ImGuiTreeNodeFlags_Selected;

        const bool opened = ImGui::TreeNodeEx((void *) GO, flags, "%s", GO->GetName().c_str());

        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("GAMEOBJECT_HIERARCHY", &GO, sizeof(GameObject *));

            ImGui::Text("Moving %s", GO->GetName().c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_HIERARCHY")) {
                GameObject *draggedGO = *static_cast<GameObject **>(payload->Data);

                if (draggedGO != GO && !GO->IsChild(draggedGO)) {
                    draggedGO->SetParent(GO, true);
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemClicked()) {
            selectedGO = GO;
        }

        if (ImGui::BeginPopupContextItem()) {
            selectedGO = GO;

            if (ImGui::MenuItem("Duplicate")) {
                DuplicateGameObject(GO, nullptr);
            }

            if (ImGui::MenuItem("Create Child")) {
                auto child = std::make_unique<GameObject>("Empty GameObject");
                GameObject *childPtr = child.get();

                auto &scene = SceneManager::GetInstance().GetSceneByIdx(0);
                scene.Add(std::move(child));

                childPtr->SetParent(GO, false);
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Delete", "Del")) {
                GameObject *toDelete = selectedGO;

                selectedGO = nullptr;
                toDelete->MarkForDeletion();
            }

            ImGui::EndPopup();
        }

        if (opened) {
            for (int idx = 0; idx < GO->GetChildCount(); ++idx) {
                VisualizeSceneGraph(GO->GetChildAt(idx));
            }
            ImGui::TreePop();
        }
    }

    void EditorGui::DuplicateGameObject(GameObject *original, GameObject *newParent) {
        if (!original) return;

        auto clone = original->Clone();
        GameObject *clonePtr = clone.get();

        auto &scene = SceneManager::GetInstance().GetSceneByIdx(0);
        scene.Add(std::move(clone));

        if (newParent) {
            clonePtr->SetParent(newParent, false);
        } else if (original->GetParent()) {
            clonePtr->SetParent(original->GetParent(), false);
        }

        clonePtr->UpdateWorldPosition();

        for (int i = 0; i < original->GetChildCount(); ++i) {
            DuplicateGameObject(original->GetChildAt(i), clonePtr);
        }

        if (newParent == nullptr) {
            selectedGO = clonePtr;
        }
    }

    void EditorGui::ClearSelection() {
        selectedGO = nullptr;
    }
#pragma endregion SceneGraph
    //Inspector
#pragma region InspectorGUI
    void EditorGui::DrawInspector(GameObject *GO) {
        ImGui::Begin("Inspector");

        if (!GO) {
            ImGui::Text("Select an object to see properties.");
            ImGui::End();
            return;
        }

        char nameBuffer[128];
#if defined(_WIN32) || defined(_WIN64)
        strcpy_s(nameBuffer, sizeof(nameBuffer), GO->GetName().c_str());
#else
        strncpy(nameBuffer, GO->GetName().c_str(), sizeof(nameBuffer) - 1);
        nameBuffer[sizeof(nameBuffer) - 1] = '\0';
#endif

        if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
            GO->SetName(nameBuffer);
        }
        bool enabled = GO->IsActive();
        if (ImGui::Checkbox("IsEnabled", &enabled)) {
            GO->SetActive(enabled);
        }
        ImGui::Separator();

        GO->GetTransform()->InspectorGUI();

        Component *componentToPendingRemoval = nullptr;

        for (auto &Comp: GO->GetComponents()) {
            if (dynamic_cast<TransformComponent *>(Comp.get())) continue;

            const bool hasWarning = Comp->HasWarning();

            if (hasWarning) {
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.f, 0.f, 0.95f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
            }

            bool isActive{Comp->GetActive()};

            ImGui::PushID(Comp.get());
            const bool open = ImGui::CollapsingHeader("##ComponentHeader",
                                                      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);

            ImGui::SameLine();
            if (ImGui::Checkbox("##Enabled", &isActive)) {
                Comp->ChangeActive(isActive);
            }

            ImGui::SameLine();
            ImGui::TextUnformatted(Comp->GetName().c_str());

            if (ImGui::BeginPopupContextItem("ComponentContextMenu")) {
                if (ImGui::MenuItem("Remove Component")) {
                    componentToPendingRemoval = Comp.get();
                }
                ImGui::EndPopup();
            }

            Comp->CheckActive();

            if (open) {
                Comp->InspectorGUI();
                ImGui::Text(" ");
            }

            Comp->EndCheckActive();
            ImGui::PopID();
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
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Available Components");
            ImGui::Separator();

            auto componentNames = ComponentFactory::GetInstance().GetRegisteredTypeNames();

            for (const std::string &name: componentNames) {
                if (ImGui::MenuItem(name.c_str())) {
                    auto newComp = ComponentFactory::GetInstance().Create(name, GO);
                    if (newComp) {
                        GO->AddComponent(std::move(newComp));
                    }
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
    }
#pragma endregion InspectorGUI
    //Top Bar
#pragma region TopBarGUI
    void EditorGui::DrawTopBar() {
        DrawSaveAsPopup();
        DrawFileBrowserPopup();

        if (ImGui::BeginMainMenuBar()) {
            // File Menu
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                    // Clear current scene
                    InputManager::GetInstance().ClearBindings();
                    ClearSelection();
                    Component::ClearIds();
                    GameObject::ClearIds();
                    SceneManager::GetInstance().GetSceneByIdx(0).ClearGameObjects();
                }
                if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) {
                    m_FileBrowser.Open();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                    SaveScene();
                }
                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
                    m_RequestSaveAsPopup = true;
                }
                ImGui::EndMenu();
            }

            // View Menu
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Scene Graph", "", &m_ShowSceneGraph);
                ImGui::MenuItem("Inspector", "", &m_ShowInspector);
                ImGui::Separator();
                if (ImGui::MenuItem("Reset Layout")) {}
                ImGui::EndMenu();
            }

            const ImGuiIO &io = ImGui::GetIO();

            if (io.WantCaptureKeyboard) {
                ImGui::Separator();
                ImGui::TextColored(ImVec4(1.f, 0.8f, 0.f, 1.0f), "(You are tabbed out of the game)");
            }

            ImGui::EndMainMenuBar();
        }
    }

    void EditorGui::DrawSaveAsPopup() {
        if (m_RequestSaveAsPopup) {
            ImGui::OpenPopup("SaveScenePopup");
            m_RequestSaveAsPopup = false;
        }

        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("SaveScenePopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char nameBuffer[64] = "NewScene";

            ImGui::Text("Enter a name for the scene:");
            ImGui::InputText("##sceneName", nameBuffer, IM_ARRAYSIZE(nameBuffer));

            ImGui::Separator();

            if (ImGui::Button("Save", ImVec2(120, 0))) {
                const std::string newName = nameBuffer;

                SaveScene(newName);

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void EditorGui::DrawFileBrowserPopup() {
        m_FileBrowser.Display();

        if (m_FileBrowser.HasSelected()) {
            const std::string path = m_FileBrowser.GetSelected().string();

            InputManager::GetInstance().ClearBindings();
            ClearSelection();

            SceneSerializer::LoadScene(
                path, SceneManager::GetInstance().GetSceneByIdx(0));

            m_FileBrowser.ClearSelected();
        }
    }

    void EditorGui::SaveScene() {
        const std::filesystem::path scenesDir = std::filesystem::current_path() / "Data" / "Scenes";
        std::filesystem::create_directories(scenesDir);

        const std::string sceneName = SceneManager::GetInstance().GetSceneByIdx(
            SceneManager::GetInstance().GetCurrentSceneIdx()).GetName();

        const std::filesystem::path filePath = scenesDir / (sceneName + ".json");

        SceneSerializer::GetInstance().SaveScene(filePath.string(),
                                                 SceneManager::GetInstance().GetSceneByIdx(
                                                     SceneManager::GetInstance().GetCurrentSceneIdx()));

        SDL_Log("Saved to %s", filePath.string().c_str());
    }

    void EditorGui::SaveScene(const std::string &sceneName) {
        const std::filesystem::path scenesDir = std::filesystem::current_path() / "Data" / "Scenes";
        std::filesystem::create_directories(scenesDir);

        const std::filesystem::path filePath = scenesDir / (sceneName + ".json");
        SceneManager::GetInstance().GetSceneByIdx(SceneManager::GetInstance().GetCurrentSceneIdx()).SetName(sceneName);

        SceneSerializer::GetInstance().SaveScene(filePath.string(),
                                                 SceneManager::GetInstance().GetSceneByIdx(
                                                     SceneManager::GetInstance().GetCurrentSceneIdx()));

        SDL_Log("Saved to %s", filePath.string().c_str());
    }
#pragma endregion TopBarGUI
}
