#include "DamageComponent.h"
#include "GameObject.h"
#include "HamburgerMovementComponent.h"
#include "SDL3/SDL_log.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/ColliderComponent.h"
#include "Singletons/SceneManager.h"

namespace game {
    void DamageComponent::Update() {
        for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                 ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
            if (obj.get() == m_gameObject) continue;

            const auto *player = obj->GetComponent<ge::CharacterControllerComponent>();
            if (!player) continue;

            const auto *otherCollider = obj->GetComponent<ge::ColliderComponent>();
            if (!otherCollider) continue;
            const auto myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();

            if (myCollider->IsOverlapping(otherCollider)) {
                if (player->GetControllerIndex() == 0) {
                    ge::EventManager::GetInstance().SendEvent(ge::Event(m_SendEventId1));
                } else if (player->GetControllerIndex() == 1) {
                    ge::EventManager::GetInstance().SendEvent(ge::Event(m_SendEventId1));
                }
            }
        }
    }

    void DamageComponent::InspectorGUI() {}

    nlohmann::ordered_json DamageComponent::Serialize() const {
        return {};
    }

    void DamageComponent::Deserialize(const nlohmann::ordered_json &/*data*/) {}
}
