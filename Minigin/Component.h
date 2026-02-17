#pragma once
#include <memory>

namespace dae {
    class GameObject;

    class Component {
    public:
        Component(GameObject *go) : m_gameObject(go) {}
        virtual ~Component() = default;
        Component(const Component &other) = delete;
        Component(Component &&other) = delete;
        Component &operator=(const Component &other) = delete;
        Component &operator=(Component &&other) = delete;
        virtual void Update() = 0;
        virtual void Render() const = 0;

        GameObject *m_gameObject;
    protected:
    };
}
