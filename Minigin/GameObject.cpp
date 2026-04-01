#include "GameObject.h"
#include <algorithm>
#include <cassert>
#include <utility>
#include "Components/Component.h"
#include "Components/TransformComponent.h"
using namespace dae;
GameObject::GameObject(std::string name) : m_Name(std::move(name)) {}
GameObject::~GameObject() = default;

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

void GameObject::AddComponent(std::unique_ptr<Component> component) {
    assert(component != nullptr);
    m_Components.push_back(std::move(component));
}

void GameObject::RemoveComponent(Component *component) {
    m_Components.erase(
        std::remove_if(
            m_Components.begin(), m_Components.end(),
            [component](const auto &comp) { return comp.get() == component; }),
        m_Components.end());
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
