#pragma once
#include "Updatable.h"
#include "States/GSManager.h"

class GameUpdater : public ge::Updatable {
public:
    void Update() override {
        game::GSManager::GetInstance().Update();
    }
};
