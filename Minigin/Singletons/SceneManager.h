#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace ge {
    class Scene;

    class SceneManager final : public Singleton<SceneManager> {
    public:
        Scene &CreateScene(std::string name);
        [[nodiscard]] Scene &GetSceneByName(const std::string &name) const;
        [[nodiscard]] Scene &GetSceneByIdx(int idx) const;
        [[nodiscard]] std::vector<int> GetSceneSizes() const;
        [[nodiscard]] const std::vector<std::unique_ptr<Scene> > &GetScenes() const;
        void SelectSceneByIndex(int idx);
        void SelectSceneByName(const std::string &name);
        void Update();
        void Render() const;
        void RenderGUI() const;
        int GetCurrentSceneIdx() const;

    private:
        friend class Singleton<SceneManager>;
        SceneManager() = default;
        std::vector<std::unique_ptr<Scene> > m_scenes{};
        int m_SceneIdx {0};
    };
}
