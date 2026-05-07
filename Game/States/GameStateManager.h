#pragma once
#include "GameState.h"
#include "Singletons/Singleton.h"

namespace dae {
    class GameStateManager final : public Singleton<GameStateManager> {
    public:
        void ChangeState(GameState *newState);
        void Update();
        void Render() const;

    private:
        friend Singleton<GameStateManager>;
        GameState *m_CurrentGameState;
    };
}
