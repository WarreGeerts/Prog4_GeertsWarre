#pragma once
#include "GameState.h"
#include "Singletons/EventManager.h"

namespace dae {
    class GSMainMenu final : public GameState {
    public:
        void OnEnter() override;
        void OnExit() override;
        void HandleInput() override;
        void Update() override{};
    private:
        EventHandle m_Handle;
    };
}
