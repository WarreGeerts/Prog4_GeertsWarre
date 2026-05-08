#pragma once
#include <memory>

#include "GameState.h"
#include "Singletons/Singleton.h"

namespace game {
    class GSManager final : public ge::Singleton<GSManager> {
    public:
        void Initialize();
        void ChangeState(std::unique_ptr<GameState> newState);
        void Update();

    private:
        friend Singleton<GSManager>;
        std::unique_ptr<GameState> m_CurrentGameState;
    };
}
