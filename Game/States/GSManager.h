#pragma once
#include "GameState.h"
#include "GSLobby.h"
#include "GSMainMenu.h"
#include "Singletons/Singleton.h"

namespace dae {
    class GSManager final : public Singleton<GSManager> {
    public:
        void Initialize();

        void ChangeState(GameState *newState);
        void Update();

    private:
        friend Singleton<GSManager>;
        GameState *m_CurrentGameState = nullptr;
    };
}
