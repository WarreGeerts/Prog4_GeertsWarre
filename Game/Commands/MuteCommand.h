#pragma once
#include "Commands/Command.h"

namespace game {
    class MuteCommand : public ge::Command {
    public:
        explicit MuteCommand();
        void Execute() override;

    private:
        float m_Volume{1};
    };
}
