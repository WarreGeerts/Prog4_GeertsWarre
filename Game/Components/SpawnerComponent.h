#pragma once
#include "glm/vec2.hpp"
#include "Components/Component.h"

namespace game {
    class SpawnerComponent final : public ge::Component {
    public:
        explicit SpawnerComponent(ge::GameObject* go) : Component(go, "SpawnerComponent") {};
        void Update() override;
        void Render() const override{};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        bool m_Start = false;
        bool m_Deserialized = false;
        glm::vec2 m_SpawnPosition = { 0.0f, 0.0f };
        std::string m_LinkedGOName{};

    };
}
