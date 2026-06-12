#pragma once
#include "Commands/Command.h"

namespace game {
    class SkipLevelCommand : public ge::Command {
    public:
        explicit SkipLevelCommand();
        void Execute() override;

    private:
    };
}
