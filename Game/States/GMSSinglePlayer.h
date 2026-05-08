#pragma once
#include "GameModeState.h"

namespace game {
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
