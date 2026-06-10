#include "HamburgerMovementComponent.h"
#include <SDL3/SDL_log.h>
#include "HamburgerHolderComponent.h"
#include "Components/ColliderComponent.h"
#include "Singletons/DeltaTime.h"
#include "Singletons/SceneManager.h"

namespace game {
    void HamburgerMovementComponent::Update() {
        if (!m_IsFalling) return;

        auto *myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();

        glm::vec3 pos = m_gameObject->GetWorldPosition();
        pos.y += ge::DeltaTime::GetInstance().Time() * m_FallSpeed;
        m_gameObject->SetLocalPosition(pos);

        if (!myCollider) return;

        for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                 ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
            if (obj.get() == m_gameObject) continue;

            auto *holder = obj->GetComponent<HamburgerHolderComponent>();
            if (!holder) continue;

            auto *otherCollider = obj->GetComponent<ge::ColliderComponent>();
            if (!otherCollider) continue;

            if (myCollider->IsOverlapping(otherCollider)) {
                if (holder == m_IgnoredHolder) continue;

                m_IsFalling = false;
                m_IgnoredHolder = nullptr;

                const auto myBounds = myCollider->GetWorldBounds();
                const auto otherBounds = otherCollider->GetWorldBounds();

                const float colliderOffsetY = myBounds.y - pos.y;
                pos.y = otherBounds.y - myBounds.w - colliderOffsetY;
                m_gameObject->SetLocalPosition(pos);

                holder->IsFinishedFalling(m_gameObject);
                break;
            }
        }
    }

    void HamburgerMovementComponent::StartFalling(HamburgerHolderComponent *fromHolder) {
        m_IsFalling = true;
        m_IgnoredHolder = fromHolder;
    }

    nlohmann::ordered_json HamburgerMovementComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["fallSpeed"] = m_FallSpeed;
        return data;
    }

    void HamburgerMovementComponent::Deserialize(const nlohmann::ordered_json &json) {
        m_FallSpeed = json.value("fallSpeed", 100.f);
    }
    void HamburgerMovementComponent::InspectorGUI() {
        ImGui::InputFloat("Fall Speed", &m_FallSpeed);

    }
}
