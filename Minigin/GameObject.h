#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Components/TransformComponent.h"
#include "glm/vec3.hpp"

namespace ge {
    class Texture2D;
    class Component;

    class GameObject final {
    public:
        explicit GameObject(std::string name);
        //important functions
        void Update();
        [[nodiscard]] bool MarkedForDeletion() const { return m_MarkedForDeletion; }
        void MarkForDeletion();
        [[nodiscard]] const std::string &GetName() const { return m_Name; }
        void SetName(const std::string &name) { m_Name = name; };
        void SetActive(bool value);
        [[nodiscard]] bool IsActive() const;
        [[nodiscard]] bool GetIsEnabled() const { return m_IsEnabled; }
        //components
        [[nodiscard]] const std::vector<std::unique_ptr<Component> > &GetComponents() const { return m_Components; }
        [[nodiscard]] const std::unique_ptr<TransformComponent> &GetTransform() const { return m_Transform; }
        void AddComponent(std::unique_ptr<Component> component);
        void RemoveComponent(Component *component);

        template<typename T>
        T *GetComponent() const {
            for (const auto &component: m_Components) {
                if (auto *casted_comp = dynamic_cast<T *>(component.get())) {
                    return casted_comp;
                }
            }
            return nullptr;
        }

        template<typename T>
        [[nodiscard]] bool HasComponent() const { return GetComponent<T>() != nullptr; }

        void NoteChange() { m_ComponentListVersion++; }
        [[nodiscard]] uint32_t GetComponentListVersion() const { return m_ComponentListVersion; }
        //Parent-child functions
        void SetParent(GameObject *parent, bool keepWorldPosition);
        [[nodiscard]] GameObject *GetParent() const { return m_Parent; }
        [[nodiscard]] int GetChildCount() const { return static_cast<int>(m_Children.size()); }
        [[nodiscard]] GameObject *GetChildAt(const int index) const { return m_Children[index]; }
        bool IsChild(GameObject *object) const;
        //positions
        void SetLocalPosition(const glm::vec3 &position);
        const glm::vec3 &GetWorldPosition();
        [[nodiscard]] const glm::vec3 &GetLocalPosition() const { return m_LocalPosition; }
        void UpdateWorldPosition();
        void SetPositionDirty();
        void AddIntendedMovement(const glm::vec3& displacement) { m_IntendedMovement += displacement; }
        [[nodiscard]] const glm::vec3& GetIntendedMovement() const { return m_IntendedMovement; }
        void ClearIntendedMovement() { m_IntendedMovement = glm::vec3(0.0f); }
        //default constructor stuff
        ~GameObject();
        GameObject(const GameObject &other) = delete;
        GameObject(GameObject &&other) = delete;
        GameObject &operator=(const GameObject &other) = delete;
        GameObject &operator=(GameObject &&other) = delete;
        //id
        [[nodiscard]] int GetId() const { return m_Id; }
        static void ClearIds();
        //editor
        [[nodiscard]] std::unique_ptr<GameObject> Clone() const;

    private:
        //general vars
        std::vector<std::unique_ptr<Component> > m_Components{};
        std::unique_ptr<TransformComponent> m_Transform{std::make_unique<TransformComponent>(this)};
        std::string m_Name;
        bool m_MarkedForDeletion{false};
        uint32_t m_ComponentListVersion{0};
        bool m_IsEnabled{true};
        //parent-child vars
        GameObject *m_Parent{nullptr};
        std::vector<GameObject *> m_Children{};
        //positions
        glm::vec3 m_LocalPosition{};
        glm::vec3 m_WorldPosition{};
        bool m_PositionIsDirty{true};
        glm::vec3 m_IntendedMovement{0.f, 0.f, 0.f};
        //parent-child functions
        void AddChild(GameObject *child);
        void RemoveChild(const GameObject *child);
        //id
        int m_Id;
        static int s_NextId;
        static int GenerateNextId() { return s_NextId++; }
    };
}
