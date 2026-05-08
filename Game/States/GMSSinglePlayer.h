#pragma once
#include "GameModeState.h"
#include "Singletons/EventManager.h"

namespace dae {
    class GMSSinglePlayer final : public GameModeState {
    public:
        void OnEnter() override;
        void OnExit() override;
        void Update() override;
        void HandleInput() override;
        void Render() override;

    private:
    };
}
