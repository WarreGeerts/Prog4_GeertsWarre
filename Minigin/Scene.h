#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

namespace dae {
    class Scene final {
    public:
        void Add(std::unique_ptr<GameObject> object);
        void Remove(const GameObject &object);
        void RemoveAll();
        [[nodiscard]] GameObject &GetGameObjectByName(const std::string &name) const;
        [[nodiscard]] GameObject &GetGameObjectByIndex(int idx) const;
        [[nodiscard]] const std::vector<std::unique_ptr<GameObject> > &GetGameObjects() const;
        [[nodiscard]] int GetSceneSize() const;
        std::string &GetName() { return m_Name; }
        void Update();
        void Render() const;
        void RenderGUI() const;
        ~Scene() = default;
        Scene(const Scene &other) = delete;
        Scene(Scene &&other) = delete;
        Scene &operator=(const Scene &other) = delete;
        Scene &operator=(Scene &&other) = delete;

    private:
        friend class SceneManager;
        explicit Scene(std::string name) { m_Name = std::move(name); }
        std::vector<std::unique_ptr<GameObject> > m_objects{};
        std::string m_Name;
    };
}
