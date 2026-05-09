#pragma once
#include "GameState.h"
#include "Singletons/EventManager.h"

namespace game {
    class GSLobby final : public GameState {
    public:
        void OnEnter() override;
        void OnExit() override;
        void HandleInput() override;
        void Update() override{};

    private:
        ge::EventHandle m_HandleSP;
        ge::EventHandle m_HandleCO;
        ge::EventHandle m_HandleVS;
    };
}
