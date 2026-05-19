#pragma once
#include "PlayerState.h"

namespace game {
    class PSClimbing final : public PlayerState {
    public:
        ~PSClimbing() override = default;
        void OnEnter(PlayerStateControllerComponent *owner) override;
        void OnUpdate(PlayerStateControllerComponent *owner) override;
        void OnExit(PlayerStateControllerComponent *owner) override;
    };
}
