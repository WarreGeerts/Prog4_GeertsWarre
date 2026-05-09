#pragma once
#include "GameState.h"

namespace game {
    class GSGame : public GameState {
    public:
        void OnEnter() override{};
        void OnExit() override{};
        void HandleInput() override{};
        void Update() override{};

    private:
    };
}
