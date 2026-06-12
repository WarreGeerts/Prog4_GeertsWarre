#include "HamburgerMovementComponent.h"
#include <SDL3/SDL_log.h>
#include "EnemyComponent.h"
#include "HamburgerHolderComponent.h"
#include "Components/ColliderComponent.h"
#include "Singletons/DeltaTime.h"
#include "Singletons/SceneManager.h"

namespace game {
    void HamburgerMovementComponent::Update() {
        if (!m_IsFalling) return;

        glm::vec3 pos = m_gameObject->GetWorldPosition();
        pos.y += ge::DeltaTime::GetInstance().Time() * m_FallSpeed;
        m_gameObject->SetLocalPosition(pos);

        auto *myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();
        if (!myCollider) return;

        for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                 ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
            auto *otherCollider = obj->GetComponent<ge::ColliderComponent>();
            if (!otherCollider || !myCollider->IsOverlapping(otherCollider)) continue;

            if (obj->HasComponent<EnemyComponent>()) {
                if (m_HitEnemies.find(obj.get()) == m_HitEnemies.end()) {
                    m_HitEnemies.insert(obj.get());
                    m_AmountOfDrops++;
                }
            }

            auto *holder = obj->GetComponent<HamburgerHolderComponent>();
            if (holder && holder != m_IgnoredHolder) {
                const auto myBounds = myCollider->GetWorldBounds();
                const auto otherBounds = otherCollider->GetWorldBounds();
                pos.y = otherBounds.y - myBounds.w - (myBounds.y - pos.y);
                m_gameObject->SetLocalPosition(pos);

                holder->IsFinishedFalling(m_gameObject, m_AmountOfDrops);
                m_IsFalling = false;
                m_AmountOfDrops = 0;
                m_HitEnemies.clear();
                m_IgnoredHolder = nullptr;
                break;
            }
        }
    }

    void HamburgerMovementComponent::StartFalling(HamburgerHolderComponent *fromHolder, int initialDrops) {
        m_IsFalling = true;
        m_IgnoredHolder = fromHolder;
        m_AmountOfDrops = initialDrops;
        m_HitEnemies.clear();
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