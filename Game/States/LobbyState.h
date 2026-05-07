#pragma once
#include "GameState.h"

class LobbyState final : public GameState {
    public:
    void OnEnter() override{};
    void OnExit() override{};
    void HandleInput() override{};
    void Update() override{};
    void Render() override{};
};
