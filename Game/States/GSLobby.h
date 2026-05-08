#pragma once
#include "GameState.h"

namespace game {
    class GSLobby final : public GameState {
    public:
        void OnEnter() override;
        void OnExit() override{};
        void HandleInput() override{};
        void Update() override{};
    };
}