#pragma once
#include "glm/vec2.hpp"
#include "Components/Component.h"

namespace game {
    class HamburgerHolderComponent final : public ge::Component {
    public:
        explicit HamburgerHolderComponent(ge::GameObject *go) : Component(go, "HamburgerHolderComponent") {};
        void Update() override;
        void Render() const override {};
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void InspectorGUI() override;
        void IsFinishedFalling(ge::GameObject *arrivedPart, int remainingDrops);
        [[nodiscard]] bool IsFull(int requiredCount) const;

    private:
        void DropPendingPart();
        bool m_FallThrough{true};
        bool m_WasTriggered{false};
        bool m_Initialized{false};
        glm::vec2 m_HighestPosition{glm::vec2(0.0f)};
        std::vector<ge::GameObject *> m_StackedParts;
        ge::GameObject* m_PendingDrop{ nullptr };
        std::string m_LinkedPartName{};
        float m_TriggerTimer = 0.0f;
        int m_RemainingDrops = 0;
    };
}
