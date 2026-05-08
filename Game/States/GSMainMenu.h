#pragma once
#include "GameState.h"
#include "Singletons/EventManager.h"

namespace game {
    class GSMainMenu final : public GameState {
    public:
        void OnEnter() override;
        void OnExit() override;
        void HandleInput() override;
        void Update() override{};
    private:
        ge::EventHandle m_Handle;
    };
}
