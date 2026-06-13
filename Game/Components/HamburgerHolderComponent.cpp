#include "HamburgerHolderComponent.h"
#include <SDL3/SDL_log.h>
#include "GameObject.h"
#include "HamburgerManager.h"
#include "HamburgerMovementComponent.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/ColliderComponent.h"
#include "Singletons/DeltaTime.h"
#include "Singletons/SceneManager.h"
#include "Sound/ServiceLocator.h"

namespace game {
    void HamburgerHolderComponent::Update() {
        if (!m_gameObject || m_gameObject->MarkedForDeletion()) return;
        const auto *myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();
        if (!myCollider) return;

        if (!m_Initialized) {
            m_Initialized = true;
            if (!m_LinkedPartName.empty()) {
                for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                         ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
                    if (obj->GetName() == m_LinkedPartName) {
                        SDL_Log("Added object: %s", m_LinkedPartName.c_str());
                        m_StackedParts.push_back(obj.get());

                        auto *otherCollider = obj->GetComponent<ge::ColliderComponent>();

                        const auto otherBounds = otherCollider->GetWorldBounds();
                        const auto colliderBounds = myCollider->GetWorldBounds();

                        glm::vec3 pos = m_gameObject->GetWorldPosition();
                        pos.y = colliderBounds.y - otherBounds.w;

                        obj->SetLocalPosition(pos);
                        break;
                    }
                }
            }

            if (!m_FallThrough) {
                auto *manager = ge::SceneManager::GetInstance().GetSceneByIdx(
                            ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjectByName(
                            "HamburgerManager")
                        .GetComponent<HamburgerManager>();
                if (manager) manager->RegisterPlate(this);
            }
        }

        if (m_TriggerTimer > 0.0f) {
            m_TriggerTimer -= ge::DeltaTime::GetInstance().Time();
            if (m_TriggerTimer <= 0.0f) {
                m_TriggerTimer = 0.0f;
                m_WasTriggered = false;
                DropPendingPart();
            }
            return;
        }

        if (!m_WasTriggered) {
            for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                     ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
                if (obj.get() == m_gameObject) continue;

                const auto *otherCollider = obj->GetComponent<ge::ColliderComponent>();
                if (!otherCollider) continue;

                if (myCollider->IsOverlapping(otherCollider) &&
                    obj->GetComponent<ge::CharacterControllerComponent>()) {
                    m_WasTriggered = true;
                    if (!m_StackedParts.empty()) {
                        m_PendingDrop = m_StackedParts.back();
                        m_StackedParts.pop_back();
                        m_RemainingDrops = 0;
                        auto &ss = ge::ServiceLocator::GetSoundSystem();
                        ss.Play(4, 0.5f);
                        ss.Play(5, 0.5f);
                        DropPendingPart();
                    }
                    break;
                }
            }
        }
    }

    nlohmann::ordered_json HamburgerHolderComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["linkedPart"] = m_LinkedPartName;
        data["fallThrough"] = m_FallThrough;

        return data;
    }

    void HamburgerHolderComponent::Deserialize(const nlohmann::ordered_json &data) {
        m_LinkedPartName = data.value("linkedPart", "");
        m_FallThrough = data.value("fallThrough", true);

        if (!m_FallThrough) {
            auto *myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();
            myCollider->SetOffset(0, 0);
        }
    }

    void HamburgerHolderComponent::InspectorGUI() {
        ImGui::Checkbox("Fall through", &m_FallThrough);

        char buf[128];
#if defined(_WIN32) || defined(_WIN64)
        strncpy_s(buf, sizeof(buf), m_LinkedPartName.c_str(), _TRUNCATE);
#else
        strncpy(buf, m_LinkedPartName.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
#endif

        if (ImGui::InputText("Linked Part##HC", buf, sizeof(buf))) {
            m_LinkedPartName = buf;
        }

        if (ImGui::Button("Reset Position")) {
            auto *myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();

            if (!m_FallThrough) {
                myCollider->SetOffset(0, 0);
            }

            m_Initialized = false;
            m_StackedParts.clear();
            m_PendingDrop = nullptr;
            m_WasTriggered = false;
            m_TriggerTimer = 0.0f;
            m_RemainingDrops = 0;

            if (!m_LinkedPartName.empty()) {
                for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                         ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
                    if (obj->GetName() == m_LinkedPartName) {
                        auto *otherCollider = obj->GetComponent<ge::ColliderComponent>();

                        const auto otherBounds = otherCollider->GetWorldBounds();
                        const auto colliderBounds = myCollider->GetWorldBounds();

                        glm::vec3 pos = m_gameObject->GetWorldPosition();
                        pos.y = colliderBounds.y - otherBounds.w;

                        obj->SetLocalPosition(pos);
                        break;
                    }
                }
            }
        }
    }

    void HamburgerHolderComponent::IsFinishedFalling(ge::GameObject *arrivedPart, int remainingDrops) {
        m_StackedParts.push_back(arrivedPart);
        m_WasTriggered = false;

        auto *myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();

        auto &ss = ge::ServiceLocator::GetSoundSystem();
        ss.Play(6, 0.5f);

        if (!m_FallThrough) {
            auto arrivedCollider = arrivedPart->GetComponent<ge::ColliderComponent>();
            myCollider->IncreaseOffset(0, -arrivedCollider->GetWorldBounds().w);
            return;
        }

        if (!m_StackedParts.empty() && m_StackedParts.front() != arrivedPart) {
            m_PendingDrop = m_StackedParts.front();
            m_StackedParts.erase(m_StackedParts.begin());
            m_RemainingDrops = remainingDrops;
            m_TriggerTimer = 0.1f;
        } else if (remainingDrops > 0) {
            m_PendingDrop = arrivedPart;
            m_StackedParts.pop_back();
            m_RemainingDrops = remainingDrops - 1;
            m_TriggerTimer = 0.1f;
        }
    }

    bool HamburgerHolderComponent::IsFull(int requiredCount) const {
        return m_StackedParts.size() >= static_cast<size_t>(requiredCount);
    }

    void HamburgerHolderComponent::DropPendingPart() {
        if (!m_PendingDrop) return;

        auto *movement = m_PendingDrop->GetComponent<HamburgerMovementComponent>();
        m_PendingDrop = nullptr;

        if (movement) {
            movement->StartFalling(this, m_RemainingDrops);
        }
    }
}
