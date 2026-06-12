#pragma once
#include "GameState.h"

namespace game {
    class GSGameOver : public GameState {
    public:
        void OnEnter() override;
        void OnExit() override;
        void HandleInput() override{};
        void Update() override{};
        void NextLevel() override{};

    private:
        int m_GameOverSceneIndex{5};
    };
}
