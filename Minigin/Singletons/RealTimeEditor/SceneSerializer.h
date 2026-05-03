#pragma once
#include <string>
#include "Singletons/Singleton.h"

namespace dae {
    class Scene;

    class SceneSerializer final : public Singleton<SceneSerializer> {
    public:
        static void SaveScene(const std::string &filePath, const Scene& scene);
        static void LoadScene(const std::string &filePath, Scene& scene);

    private:
        friend Singleton<SceneSerializer>;
    };
}

