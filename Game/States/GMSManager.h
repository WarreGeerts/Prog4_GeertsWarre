#pragma once
#include <memory>

#include "GameModeState.h"
#include "Singletons/Singleton.h"

namespace game {
    class GMSManager final : public ge::Singleton<GMSManager> {
    public:
        void Initialize();
        void ChangeState(std::unique_ptr<GameModeState> newState);
        void Update();

    private:
        friend Singleton<GMSManager>;
        std::unique_ptr<GameModeState> m_CurrentGameState;
    };
}
