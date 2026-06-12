#pragma once
#include <memory>
#include "imgui.h"
#include <string>
#include <utility>
#include <nlohmann/json.hpp>

namespace ge {
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
        void ChangeActive(const bool active) { m_IsActive = active; }
        [[nodiscard]] bool GetActive() const { return m_IsActive; }
        GameObject *GetGameObject() const { return m_gameObject; }
        //overrides
        virtual bool IsOverwritingText() { return false; };
        //Functions for EditorGui Inspector
        virtual void InspectorGUI() = 0;
        std::string GetName() { return m_Name; }
        [[nodiscard]] bool HasWarning() const { return m_HasWarning; }
        //save/load
        [[nodiscard]] virtual nlohmann::ordered_json Serialize() const = 0;
        virtual void Deserialize(const nlohmann::ordered_json &data) = 0;
        //id
        [[nodiscard]] int GetId() const { return m_Id; }
        static void ClearIds() { s_NextId = 0; }
        //active gui
        void CheckActive() const;
        void EndCheckActive() const;

    protected:
        explicit Component(GameObject *go, std::string name) : m_gameObject(go), m_Name(std::move(name)),
                                                               m_Id(GenerateNextId()) {}

        GameObject *m_gameObject;
        std::string m_Name{"_UNKNOWN_COMPONENT_"};
        uint32_t m_LastSeenVersion{0xFFFFFFFF};
        bool m_HasWarning{false};
        bool NeedsUpdate();
        //id
        int m_Id;
        static int s_NextId;
        static int GenerateNextId() { return s_NextId++; }
        //active
        bool m_IsActive{true};
    };
}
