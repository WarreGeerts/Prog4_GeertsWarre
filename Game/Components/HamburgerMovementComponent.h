#pragma once
#include "Components/Component.h"

namespace game {
    class HamburgerHolderComponent;

    class HamburgerMovementComponent final : public ge::Component {
    public:
        explicit HamburgerMovementComponent(ge::GameObject *go) : Component(go, "HamburgerMovementComponent") {};
        void Update() override;
        void StartFalling(HamburgerHolderComponent *fromHolder);
        void Render() const override {};
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &json) override;
        void InspectorGUI() override;
        bool IsFalling() const {return m_IsFalling;}

    private
    :
        bool m_IsFalling{false};
        float m_FallSpeed{100.f};
        HamburgerHolderComponent *m_IgnoredHolder{nullptr};
    };
}
