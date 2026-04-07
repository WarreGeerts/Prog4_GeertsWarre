#pragma once
#include "GameObject.h"
#include "Singletons/Singleton.h"
#include <vector>

#include "Components/TransformComponent.h"

namespace dae {
    class EditorGui : public Singleton<EditorGui> {
    public:
        void RenderGUI();

    private:
        EditorGui() = default;
        friend class Singleton<EditorGui>;

        //Scene Graph
        void DrawSceneGraph();
        void VisualizeSceneGraph(GameObject *GO);
        //Inspector
        void DrawInspector(GameObject* GO);
        void DrawAddComponentPopup(GameObject* GO);
        template<class T>
        void DrawAddComponentItem(GameObject *GO, const char *label);
    };
}

