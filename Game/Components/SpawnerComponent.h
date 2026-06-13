#pragma once
#include "glm/vec2.hpp"
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace game {
    class SpawnerComponent final : public ge::Component {
    public:
        explicit SpawnerComponent(ge::GameObject *go) : Component(go, "SpawnerComponent") {};
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void Respawn() { m_Respawn = true; }

    private:
        void SetDeathListener(ge::EventId deathEventId);
        bool m_Start = false;
        bool m_Deserialized = false;
        glm::vec2 m_SpawnPosition = {0.0f, 0.0f};
        std::string m_LinkedGOName{};
        ge::GameObject *m_linkedGo{};
        bool m_Respawn = false;

        ge::EventHandle m_Handle;
        bool m_ShouldRespawnAll = false;
        ge::EventId m_DeathEventId = static_cast<ge::EventId>(-1);
    };
}
