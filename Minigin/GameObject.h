#pragma once
#include <memory>
#include <vector>

namespace dae {
    class Texture2D;
    class Component;

    class GameObject final {
    public:
        GameObject(std::string  name);
        //important functions
        void Update();
        bool MarkedForDeletion() const { return m_markedForDeletion; }
        const std::vector<std::unique_ptr<Component> > &GetComponents() const { return m_components; }
        const std::string &GetName() const { return m_Name; }
        //components
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
        bool HasComponent() const { return GetComponent<T>() != nullptr; }

        //default constructor stuff
        ~GameObject();
        GameObject(const GameObject &other) = delete;
        GameObject(GameObject &&other) = delete;
        GameObject &operator=(const GameObject &other) = delete;
        GameObject &operator=(GameObject &&other) = delete;

    private:
        std::vector<std::unique_ptr<Component> > m_components{};
        bool m_markedForDeletion = false;
        std::string m_Name;
    };
}
