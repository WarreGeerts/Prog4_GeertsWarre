#pragma once
#include <memory>

namespace dae {
    class GameObject;

    class  Component {
    public:
        virtual ~Component() = default;
        Component(const Component &other) = delete;
        Component(Component &&other) = delete;
        Component &operator=(const Component &other) = delete;
        Component &operator=(Component &&other) = delete;
        virtual void Update() = 0;
        virtual void Render() const = 0;
        virtual void RenderGUI() = 0;

    protected:
        explicit Component(GameObject *go) : m_gameObject(go) {}
        GameObject *m_gameObject;
    };
}
