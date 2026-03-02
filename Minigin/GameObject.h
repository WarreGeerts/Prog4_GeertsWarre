#pragma once
#include <memory>
#include <vector>
#include <string>
#include "glm/vec3.hpp"

namespace dae {
    class Texture2D;
    class Component;

    class GameObject final {
    public:
        explicit GameObject(std::string name);
        //important functions
        void Update();
        [[nodiscard]] bool MarkedForDeletion() const { return m_markedForDeletion; }
        [[nodiscard]] const std::string &GetName() const { return m_Name; }
        //components
        [[nodiscard]] const std::vector<std::unique_ptr<Component> > &GetComponents() const { return m_components; }
        void AddComponent(std::unique_ptr<Component> component);
        void RemoveComponent(Component *component);

        template<typename T>
        T *GetComponent() const {
            for (const auto &component: m_components) {
                if (auto *casted_comp = dynamic_cast<T *>(component.get())) {
                    return casted_comp;
                }
            }
            return nullptr;
        }

        template<typename T>
        [[nodiscard]] bool HasComponent() const { return GetComponent<T>() != nullptr; }

        //Parent-child functions
        void SetParent(GameObject *parent,bool keepWorldPosition);
        [[nodiscard]] GameObject *GetParent() const { return m_Parent; }
        [[nodiscard]] int GetChildCount() const { return static_cast<int>(m_children.size()); }
        [[nodiscard]] GameObject *GetChildAt(const int index) const { return m_children[index]; }
        //positions
        void SetLocalPosition(const glm::vec3 &position);
        const glm::vec3 &GetWorldPosition();
        void UpdateWorldPosition();
        void SetPositionDirty();
        //default constructor stuff
        ~GameObject();
        GameObject(const GameObject &other) = delete;
        GameObject(GameObject &&other) = delete;
        GameObject &operator=(const GameObject &other) = delete;
        GameObject &operator=(GameObject &&other) = delete;

    private:
        //general vars
        std::vector<std::unique_ptr<Component> > m_components{};
        bool m_markedForDeletion = false;
        std::string m_Name;
        //parent-child vars
        GameObject *m_Parent{nullptr};
        std::vector<GameObject *> m_children{};
        //positions
        glm::vec3 m_LocalPosition{};
        glm::vec3 m_WorldPosition{};
        bool m_PositionIsDirty{};
        //parent-child functions
        void AddChild(GameObject *child);
        void RemoveChild(const GameObject *child);
        bool IsChild(GameObject *object) const;
    };
}
