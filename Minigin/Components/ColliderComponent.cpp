#include "ColliderComponent.h"
#include "Singletons/Renderer.h"

namespace ge {
    ColliderComponent::ColliderComponent(GameObject *go) : Component(go, "ColliderComponent") {}

    void ColliderComponent::Render() const {
        if (!m_IsActive) return;

        if (!m_Debug) return;

        SDL_Renderer *renderer{Renderer::GetInstance().GetSDLRenderer()};

        const glm::vec2 ownerPos = m_gameObject->GetTransform()->GetPosition();

        SDL_FRect rect;
        rect.x = ownerPos.x + m_Offset.x;
        rect.y = ownerPos.y + m_Offset.y;
        rect.w = m_Width;
        rect.h = m_Height;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &rect);
    }

    void ColliderComponent::InspectorGUI() {
        ImGui::Checkbox("Show Debug Colliders", &m_Debug);

        float offset[2] = {m_Offset.x, m_Offset.y};
        if (ImGui::InputFloat2("Offset##CC", offset)) {
            m_Offset.x = offset[0];
            m_Offset.y = offset[1];
        }
        ImGui::InputFloat("Width##CC", &m_Width);
        ImGui::InputFloat("Height##CC", &m_Height);
    }

    nlohmann::ordered_json ColliderComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["offset"] = {
            {"x", m_Offset.x},
            {"y", m_Offset.y}
        };
        data["width"] = m_Width;
        data["height"] = m_Height;
        return data;
    }

    void ColliderComponent::Deserialize(const nlohmann::ordered_json &data) {
        if (data.contains("offset")) {
            auto &offset = data["offset"];
            m_Offset.x = offset.value("x", 0.0f);
            m_Offset.y = offset.value("y", 0.0f);
        }
        m_Width = data.value("width", 0.f);
        m_Height = data.value("height", 0.f);
    }

    glm::vec4 ColliderComponent::GetWorldBounds() const {
        const glm::vec2 pos = m_gameObject->GetTransform()->GetPosition();
        return {pos.x + m_Offset.x, pos.y + m_Offset.y, m_Width, m_Height};
    }

    bool ColliderComponent::IsColliding(const float worldX, const float worldY) const {
        if (!m_IsActive) return false;

        const auto pos = GetWorldBounds();
        return (worldX >= pos.x && worldX <= pos.x + pos.z &&
                worldY >= pos.y && worldY <= pos.y + pos.w);
    }

    bool ColliderComponent::IsOverlapping(const ColliderComponent *other) const {
        if (!m_IsActive || !other->m_IsActive) return false;

        const glm::vec4 boundsA = GetWorldBounds();
        const glm::vec4 boundsB = other->GetWorldBounds();

        return (boundsA.x < boundsB.x + boundsB.z &&
                boundsA.x + boundsA.z > boundsB.x &&
                boundsA.y < boundsB.y + boundsB.w &&
                boundsA.y + boundsA.w > boundsB.y);
    }

    void ColliderComponent::IncreaseOffset(float width, float height) {
        m_Offset.x += width;
        m_Offset.y += height;
    }

    void ColliderComponent::SetOffset(float width, float height) {
        m_Offset.x = width;
        m_Offset.y = height;
    }
}
