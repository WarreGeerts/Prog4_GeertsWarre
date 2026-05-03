#pragma once
#include "GameObject.h"
#include "Singletons/Singleton.h"
#include <vector>
#include "Components/TransformComponent.h"
#include "imfilebrowser.h"

namespace dae {
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
        //Inspector
        void DrawInspector(GameObject *GO);
        void DrawAddComponentPopup(GameObject *GO);
        template<class T>
        void DrawAddComponentItem(GameObject *GO, const char *label);
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
