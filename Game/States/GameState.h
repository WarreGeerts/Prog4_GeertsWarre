#pragma once
namespace game {
    class GameState {
    public:
        virtual ~GameState() = default;
        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;
        virtual void NextLevel() {};
        virtual void HandleInput() = 0;
        virtual void Update() = 0;
    };
}
