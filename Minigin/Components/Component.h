#pragma once
#include <memory>
#include "imgui.h"
#include <string>

namespace dae {
    class GameObject;

    class Component {
    public:
        virtual ~Component() = default;
        Component(const Component &other) = delete;
        Component(Component &&other) = delete;
        Component &operator=(const Component &other) = delete;
        Component &operator=(Component &&other) = delete;
        virtual void Update() = 0;
        virtual void Render() const = 0;
        virtual void RenderGUI() {}
        //Functions for EditorGui Inspector
        virtual void InspectorGUI() {}
        std::string GetName() { return m_Name; }
        bool HasWarning() const { return m_HasWarning; }

    protected:
        explicit Component(GameObject *go, const std::string &name) : m_gameObject(go), m_Name(name) {}
        GameObject *m_gameObject;
        std::string m_Name{"_UNKNOWN_COMPONENT_"};
        uint32_t m_LastSeenVersion{0xFFFFFFFF};
        bool m_HasWarning{false};
        bool NeedsUpdate();
    };
}
