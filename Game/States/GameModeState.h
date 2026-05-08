#pragma once

namespace game {
    class GameModeState {
    public:
        virtual ~GameModeState() = default;
        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;

        virtual void HandleInput() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
    };
}