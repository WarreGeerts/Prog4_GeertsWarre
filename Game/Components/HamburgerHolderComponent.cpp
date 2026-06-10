#include "HamburgerHolderComponent.h"
#include <SDL3/SDL_log.h>
#include "GameObject.h"
#include "HamburgerMovementComponent.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/ColliderComponent.h"
#include "Singletons/SceneManager.h"

namespace game {
    void HamburgerHolderComponent::Update() {
        if (!m_Initialized) {
            m_Initialized = true;
            if (!m_LinkedPartName.empty()) {
                for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                         ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
                    if (obj->GetName() == m_LinkedPartName) {
                        SDL_Log("Added object: %s", m_LinkedPartName.c_str());
                        m_StackedParts.push_back(obj.get());
                        break;
                    }
                }
            }
        }

        const auto *myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();
        if (!myCollider) return;

        for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                 ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
            if (obj.get() == m_gameObject) continue;

            const auto *otherCollider = obj->GetComponent<ge::ColliderComponent>();
            if (!otherCollider) continue;

            if (!myCollider->IsOverlapping(otherCollider)) continue;

            if (!m_WasTriggered && obj->GetComponent<ge::CharacterControllerComponent>()) {
                m_WasTriggered = true;
                DropObject();
            }
        }
    }

    nlohmann::ordered_json HamburgerHolderComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["linkedPart"] = m_LinkedPartName;
        data["fallThrough"] = m_FallThrough;

        return data;
    }

    void HamburgerHolderComponent::Deserialize(const nlohmann::ordered_json &json) {
        m_LinkedPartName = json.value("linkedPart", "");
        m_FallThrough = json.value("fallThrough", true);
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
    }

    void HamburgerHolderComponent::IsFinishedFalling(ge::GameObject *arrivedPart) {
        m_WasTriggered = true;
        if (std::find(m_StackedParts.begin(), m_StackedParts.end(), arrivedPart) != m_StackedParts.end())
            return;

        m_StackedParts.push_back(arrivedPart);
        m_WasTriggered = false;

        if (m_FallThrough && m_StackedParts.size() > 1) {
            DropObject();
        }

        if (!m_FallThrough) {
            auto arrivedCollider = arrivedPart->GetComponent<ge::ColliderComponent>();
            m_gameObject->GetComponent<ge::ColliderComponent>()->
                    IncreaseOffset(0, -arrivedCollider->GetWorldBounds().w);
        }
    }

    void HamburgerHolderComponent::DropObject() {
        if (m_StackedParts.empty()) return;
        const auto droppedPart = m_StackedParts[0];
        m_StackedParts.erase(m_StackedParts.begin());
        m_WasTriggered = false;

        auto *movement = droppedPart->GetComponent<HamburgerMovementComponent>();
        if (movement) {
            movement->StartFalling(this);
        }
    }
}
