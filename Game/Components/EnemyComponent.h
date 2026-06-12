#pragma once
#include "GameObject.h"
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace game {
    class EnemyComponent final : public ge::Component {
    public:
        explicit EnemyComponent(ge::GameObject *go) : Component(go, "EnemyComponent") {};
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        bool Dead() const {return (m_Health <= 0);}

    private:
        int m_Health{1};
        int m_MaxHealth{1};
        float m_AccTime{0};
        float m_RespawnTime{2};
        ge::EventId m_ScoreEventId = -1;
        int m_PointsMultiplier {1};
    };
}
