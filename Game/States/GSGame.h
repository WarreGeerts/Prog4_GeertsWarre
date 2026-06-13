#pragma once
#include "GameState.h"

namespace game {
    class GSGame : public GameState {
    public:
        void OnEnter() override;
        void OnExit() override {};
        void HandleInput() override {};
        void Update() override;
        void NextLevel() override;
        void PerformLevelTransition();
        void RegisterDebugBindings();

    private:
        float m_WaitTimer{};
        bool m_MusicStarted{false};
        int m_CurrentLevelIndex{};
        int m_LowestLevelIndex{1};
        int m_HighestLevelIndex{3};
        bool m_TransitionRequested{false};
        bool m_IsTransitioning{false};
    };
}
