#pragma once
#include "GameObject.h"
#include "Singletons/Singleton.h"
#include <vector>
#include "Components/TransformComponent.h"
#include "imfilebrowser.h"

namespace ge {
    class EditorGui : public Singleton<EditorGui> {
    public:
        void RenderGUI();
        static void ClearSelection();

    private:
        EditorGui();
        friend class Singleton<EditorGui>;
        //Scene Graph
        static void DrawSceneGraph();
        static void VisualizeSceneGraph(GameObject *GO);
        static void DuplicateGameObject(const GameObject *original, GameObject *newParent);
        //Inspector
        static void DrawInspector(GameObject *GO);
        static void DrawAddComponentPopup(GameObject *GO);

        template<class T>
        void DrawAddComponentItem(GameObject *GO, const char *label) {
            const bool hasComp = GO->HasComponent<T>();

            if (ImGui::MenuItem(label, nullptr, false, !hasComp)) {
                GO->AddComponent(std::make_unique<T>(GO));
                ImGui::CloseCurrentPopup();
            }

            if (hasComp && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::SetTooltip("Object already has this component.");
            }
        }

        //top-bar
        void DrawTopBar();
        void DrawSaveAsPopup();
        void DrawFileBrowserPopup();
        static void SaveScene();
        static void SaveScene(const std::string &sceneName);
        bool m_ShowSceneGraph{true};
        bool m_ShowInspector{true};
        bool m_RequestSaveAsPopup{false};
        ImGui::FileBrowser m_FileBrowser;
    };
}
