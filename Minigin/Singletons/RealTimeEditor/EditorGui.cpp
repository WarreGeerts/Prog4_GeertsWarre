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

using namespace dae;
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
//TODO: Scene name change to file name

void EditorGui::DrawSceneGraph() {
    ImGui::Begin("Scene Graph");

    if (ImGui::BeginPopupContextWindow("SceneGraphContext",
                                       ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
        if (ImGui::MenuItem("Create Empty GameObject")) {
            auto &scene = SceneManager::GetInstance().GetSceneByIdx(0);
            scene.Add(std::make_unique<GameObject>("Empty GameObject"));
        }
        ImGui::EndPopup();
    }

    for (auto &getScene: SceneManager::GetInstance().GetScenes()) {
        ImGui::Text("%s", getScene->GetName().c_str());
        for (auto &GO: getScene->GetGameObjects()) {
            if (GO->GetParent() == nullptr)
                VisualizeSceneGraph(GO.get());
        }
    }
    ImGui::End();
}

void EditorGui::VisualizeSceneGraph(GameObject *GO) {
    if (!GO || GO->MarkedForDeletion()) return;
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (GO->GetChildCount() == 0) flags |= ImGuiTreeNodeFlags_Leaf;
    if (selectedGO == GO) flags |= ImGuiTreeNodeFlags_Selected;

    const bool opened = ImGui::TreeNodeEx((void *) GO, flags, "%s", GO->GetName().c_str());

    if (ImGui::IsItemClicked()) {
        selectedGO = GO;
    }

    if (ImGui::BeginPopupContextItem()) {
        selectedGO = GO;

        if (ImGui::MenuItem("Create Child")) {
            auto child = std::make_unique<GameObject>("Empty GameObject");
            GameObject *childPtr = child.get();

            auto &scene = SceneManager::GetInstance().GetSceneByIdx(0);
            scene.Add(std::move(child));

            childPtr->SetParent(GO, false);
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Delete", "Del")) {
            GameObject* toDelete = selectedGO;

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

void EditorGui::ClearSelection() {
    selectedGO = nullptr;
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
    bool enabled = GO->GetIsEnabled();
    if (ImGui::Checkbox("IsEnabled", &enabled)) {
        GO->SetIsEnabled(enabled);
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

/*void EditorGui::DrawAddComponentPopup(GameObject *GO) {
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
}*/

void EditorGui::DrawAddComponentPopup(GameObject *GO) {
    if (ImGui::BeginPopup("AddComponentPopup")) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Available Components");
        ImGui::Separator();

        auto componentNames = ComponentFactory::GetRegisteredTypeNames();

        for (const std::string& name : componentNames) {
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

        SceneSerializer::LoadScene(path, SceneManager::GetInstance().GetSceneByIdx(0));

        m_FileBrowser.ClearSelected();
    }
}

void EditorGui::SaveScene() {
    const std::filesystem::path scenesDir = std::filesystem::current_path() / "Data" / "Scenes";

    // Create directory if it doesn't exist
    std::filesystem::create_directories(scenesDir);

    const std::string sceneName = SceneManager::GetInstance().GetSceneByIdx(0).GetName();
    const std::filesystem::path filePath = scenesDir / (sceneName + ".json");

    SceneSerializer::GetInstance().SaveScene(filePath.string(),
                                             SceneManager::GetInstance().GetSceneByIdx(0));

    SDL_Log("Saved to %s", filePath.string().c_str());
}

void EditorGui::SaveScene(const std::string &sceneName) {
    const std::filesystem::path scenesDir = std::filesystem::current_path() / "Data" / "Scenes";

    // Create directory if it doesn't exist
    std::filesystem::create_directories(scenesDir);

    const std::filesystem::path filePath = scenesDir / (sceneName + ".json");

    SceneSerializer::GetInstance().SaveScene(filePath.string(),
                                             SceneManager::GetInstance().GetSceneByIdx(0));

    SDL_Log("Saved to %s", filePath.string().c_str());
}
#pragma endregion TopBarGUI
