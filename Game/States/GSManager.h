#pragma once
#include "GameState.h"
#include "Singletons/Singleton.h"

namespace game {
    class GSManager final : public ge::Singleton<GSManager> {
    public:
        void Initialize();

        void ChangeState(GameState *newState);
        void Update();

    private:
        friend Singleton<GSManager>;
        GameState *m_CurrentGameState = nullptr;
    };
}
