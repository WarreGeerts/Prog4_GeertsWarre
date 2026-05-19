#pragma once
#include "PlayerState.h"

namespace game {
    class PSIdle final : public PlayerState {
    public:
        ~PSIdle() override = default;
        void OnEnter(PlayerStateControllerComponent *owner) override;
        void OnUpdate(PlayerStateControllerComponent *owner) override;
        void OnExit(PlayerStateControllerComponent *owner) override;
    };
}
