#include "MuteCommand.h"
#include "Sound/ServiceLocator.h"

namespace game {
    MuteCommand::MuteCommand()
        : m_Volume(1.0f) {}

    void MuteCommand::Execute() {
        m_Volume *= -1;
        auto &soundSystem = ge::ServiceLocator::GetSoundSystem();
        soundSystem.SetMasterVolume(m_Volume);
    }
}
