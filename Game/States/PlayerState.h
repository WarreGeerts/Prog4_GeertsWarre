#pragma once
namespace game {
    class PlayerStateControllerComponent;

    class PlayerState {
    public:
        virtual ~PlayerState() = default;
        virtual void OnEnter(PlayerStateControllerComponent *owner) =0;
        virtual void OnUpdate(PlayerStateControllerComponent *owner) = 0;
        virtual void OnExit(PlayerStateControllerComponent *owner) =0;
    };
}
