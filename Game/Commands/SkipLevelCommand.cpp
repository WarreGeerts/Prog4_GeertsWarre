#include "SkipLevelCommand.h"

#include "SDL3/SDL_log.h"
#include "../States/GSManager.h"

namespace game {
    SkipLevelCommand::SkipLevelCommand() = default;
    void SkipLevelCommand::Execute() {
        SDL_Log("Level Clear");
        GSManager::GetInstance().NextLevel();
    }
}
