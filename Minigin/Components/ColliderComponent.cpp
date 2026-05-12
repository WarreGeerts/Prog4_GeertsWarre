#include "ColliderComponent.h"
#include "Singletons/Renderer.h"

namespace ge {
    ColliderComponent::ColliderComponent(GameObject *go) : Component(go, "ColliderComponent") {}
    void ColliderComponent::Render() const {
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
    void ColliderComponent::Deserialize(const nlohmann::ordered_json &) {}
    glm::vec4 ColliderComponent::GetWorldBounds() const {
        const auto pos = m_gameObject->GetTransform()->GetPosition();
        return {pos.x + m_Offset.x, pos.y + m_Offset.y, m_Width, m_Height};
    }

    bool ColliderComponent::IsColliding(const float worldX, const float worldY) const {
        const auto pos = GetWorldBounds();
        return (worldX >= pos.x && worldX <= pos.x + pos.z &&
                worldY >= pos.y && worldY <= pos.y + pos.w);
    }
}
