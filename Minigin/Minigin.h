#pragma once
#include <functional>
#include <filesystem>

namespace dae {
    class GameObject;
    class Scene;

    class Minigin final {
    public:
        explicit Minigin(const std::filesystem::path &dataPath);
        ~Minigin();
        void Run(const std::function<void()> &load);
        void Update();
        void FixedUpdate();
        Minigin(const Minigin &other) = delete;
        Minigin(Minigin &&other) = delete;
        Minigin &operator=(const Minigin &other) = delete;
        Minigin &operator=(Minigin &&other) = delete;

    private:
        bool m_quit{false};
        //Reference declarations ========
        Scene *mainScene;
        GameObject *FPSCounter;
    };
}
