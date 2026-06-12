#include "EnemyComponent.h"
#include "AIControllerComponent.h"
#include "HamburgerMovementComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/RenderComponent.h"
#include "Singletons/DeltaTime.h"
#include "Singletons/SceneManager.h"
#include "Sound/ServiceLocator.h"

namespace game {
    void EnemyComponent::Update() {
        if (m_Health <= 0) {
            m_gameObject->GetComponent<ge::RenderComponent>()->ChangeActive(false);
            m_gameObject->GetComponent<AIControllerComponent>()->Died();
            m_AccTime += ge::DeltaTime::GetInstance().Time();

            if (m_AccTime >= m_RespawnTime) {
                m_AccTime = 0.0f;
                m_Health = m_MaxHealth;
                m_gameObject->GetComponent<ge::RenderComponent>()->ChangeActive(true);
                m_gameObject->GetComponent<AIControllerComponent>()->Respawned();
            }
        }

        for (const auto &obj: ge::SceneManager::GetInstance().GetSceneByIdx(
                 ge::SceneManager::GetInstance().GetCurrentSceneIdx()).GetGameObjects()) {
            if (obj.get() == m_gameObject) continue;

            const auto *hamburgerPiece = obj->GetComponent<HamburgerMovementComponent>();
            if (!hamburgerPiece) continue;

            const auto *otherCollider = obj->GetComponent<ge::ColliderComponent>();
            if (!otherCollider) continue;
            const auto myCollider = m_gameObject->GetComponent<ge::ColliderComponent>();

            if (myCollider->IsOverlapping(otherCollider)) {
                if (hamburgerPiece->IsFalling()) {
                    m_Health--;
                    auto &ss = ge::ServiceLocator::GetSoundSystem();
                    ss.Play(7, 0.5f);
                    ge::EventManager::GetInstance().SendEvent(
                        ge::Event(m_ScoreEventId).AddInt(100 * m_PointsMultiplier));
                }
            }
        }
    }

    void EnemyComponent::InspectorGUI() {
        ImGui::Text("Points Multiplier:");
        ImGui::RadioButton("Mr. HotDog", &m_PointsMultiplier, 1);
        ImGui::RadioButton("Mr. Pickle", &m_PointsMultiplier, 2);
        ImGui::RadioButton("Mr. Egg", &m_PointsMultiplier, 3);

        auto &em = ge::EventManager::GetInstance();
        const std::string name = em.GetEventName(m_ScoreEventId);
        if (ImGui::BeginCombo("Score Event##HMC", name.c_str())) {
            for (auto const &[id, eventName]: em.GetRegisteredEvents()) {
                if (ImGui::Selectable(eventName.c_str(), id == m_ScoreEventId))
                    m_ScoreEventId = id;
            }
            ImGui::EndCombo();
        }
    }

    nlohmann::ordered_json EnemyComponent::Serialize() const {
        nlohmann::ordered_json data;
        data["scoreEvent"] = m_ScoreEventId;
        return data;
    }

    void EnemyComponent::Deserialize(const nlohmann::ordered_json &data) {
        m_ScoreEventId = data.value("scoreEvent", -1);
    }
}
