#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "Component.h"

namespace dae {
    class Texture2D;

    class GameObject {
    public:
        //important functions
        virtual void Update();
        virtual void Render() const;
        void SetTexture(const std::string &filename);
        bool MarkedForDeletion() const {return m_markedForDeletion;}
        const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_components; }

        //components
        void AddComponent(std::unique_ptr<Component> component);
        void RemoveComponent(Component *component);
        template<typename T>
        T *GetComponent() const;
        template<typename T>
        bool HasComponent() const { return GetComponent<T>() != nullptr; };

        //default constructor stuff
        GameObject() = default;
        virtual ~GameObject();
        GameObject(const GameObject &other) = delete;
        GameObject(GameObject &&other) = delete;
        GameObject &operator=(const GameObject &other) = delete;
        GameObject &operator=(GameObject &&other) = delete;

    private:
        //Transform m_transform{}; //SHOULD get handles by transformComponent

        std::shared_ptr<Texture2D> m_texture{};
        std::vector<std::unique_ptr<Component> > m_components{};
        bool m_markedForDeletion = false;
    };


}
