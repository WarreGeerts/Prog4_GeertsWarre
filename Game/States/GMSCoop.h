#pragma once
#include "GameModeState.h"

class GMSCoop final : public game::GameModeState {
public:
    void OnEnter() override;
    void OnExit() override;
    void Update() override{};
    void HandleInput() override{};
    void Render() override{};

private:
};
