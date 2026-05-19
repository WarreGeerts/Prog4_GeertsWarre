#pragma once
#include "PlayerState.h"

namespace game {
    class PSWalking final : public PlayerState {
    public:
        ~PSWalking() override = default;
        void OnEnter(PlayerStateControllerComponent *owner) override;
        void OnUpdate(PlayerStateControllerComponent *owner) override;
        void OnExit(PlayerStateControllerComponent *owner) override;
    };
}
