#include "GameObject.h"
#include <algorithm>
#include <cassert>
#include <utility>
#include "Components/Component.h"
#include "Components/TransformComponent.h"
#include "Singletons/RealTimeEditor/ComponentFactory.h"

namespace ge {
    int GameObject::s_NextId = 0;
    GameObject::GameObject(std::string name) : m_Name(std::move(name)), m_Id(GenerateNextId()) {}
    GameObject::~GameObject() = default;

    void GameObject::ClearIds() {
        s_NextId = 0;
    }

    std::unique_ptr<GameObject> GameObject::Clone() const {
        auto clone = std::make_unique<GameObject>(m_Name + "_Copy");

        clone->SetLocalPosition(this->m_LocalPosition);

        for (const auto &comp: m_Components) {
            auto newComp = ComponentFactory::GetInstance().Create(comp->GetName(), clone.get());
            if (newComp) {
                newComp->Deserialize(comp->Serialize());
                clone->AddComponent(std::move(newComp));
            }
        }
        return clone;
    }

    void GameObject::AddChild(GameObject *child) {
        m_Children.push_back(child);
    }

    void GameObject::RemoveChild(const GameObject *child) {
        //iterate over the vector until the child is found
        auto it = std::find(m_Children.begin(), m_Children.end(), child);
        //check if child exists in the vector
        if (it != m_Children.end()) {
            m_Children.erase(it); //erase child at iterator position
        }
    }

    bool GameObject::IsChild(GameObject *object) const {
        for (const auto child: m_Children) {
            if (child == object || child->IsChild(object)) return true;
        }
        return false;
    }

    void GameObject::Update() {
        for (const auto &component: m_Components) {
            component->Update();
        }
    }

    void GameObject::MarkForDeletion() {
        if (m_MarkedForDeletion) return;
        m_MarkedForDeletion = true;

        if (m_Parent) {
            m_Parent->RemoveChild(this);
            m_Parent = nullptr;
        }

        for (auto *child: m_Children) {
            if (child) {
                child->MarkForDeletion();
            }
        }
        m_Children.clear();
    }

    void GameObject::SetActive(const bool value) {
        m_IsEnabled = value;
    }

    bool GameObject::IsActive() const {
        if (!m_IsEnabled) return false;
        if (m_Parent) return m_Parent->IsActive();
        return true;
    }

    void GameObject::AddComponent(std::unique_ptr<Component> component) {
        assert(component != nullptr);
        m_Components.push_back(std::move(component));
        NoteChange();
    }

    void GameObject::RemoveComponent(Component *component) {
        m_Components.erase(
            std::remove_if(
                m_Components.begin(), m_Components.end(),
                [component](const auto &comp) { return comp.get() == component; }),
            m_Components.end());
        NoteChange();
    }

    void GameObject::SetParent(GameObject *parent, const bool keepWorldPosition) {
        if (IsChild(parent) || parent == this || m_Parent == parent) return;
        if (parent == nullptr)
            SetLocalPosition(GetWorldPosition());
        else {
            if (keepWorldPosition)
                SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
            SetPositionDirty();
        }
        if (m_Parent) m_Parent->RemoveChild(this);
        m_Parent = parent;
        if (m_Parent) m_Parent->AddChild(this);
    }

    void GameObject::SetLocalPosition(const glm::vec3 &position) {
        m_LocalPosition = position;
        SetPositionDirty();
    }

    const glm::vec3 &GameObject::GetWorldPosition() {
        if (m_PositionIsDirty)
            UpdateWorldPosition();
        return m_WorldPosition;
    }

    void GameObject::UpdateWorldPosition() {
        if (m_PositionIsDirty) {
            if (m_Parent == nullptr)
                m_WorldPosition = m_LocalPosition;
            else
                m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;

            if (m_Transform != nullptr)
                m_Transform->SetPosition(m_WorldPosition);
        }
        m_PositionIsDirty = false;
    }

    void GameObject::SetPositionDirty() {
        m_PositionIsDirty = true;

        //Set all children dirty
        for (auto *child: m_Children) {
            if (child) {
                child->SetPositionDirty();
            }
        }
    }
}
