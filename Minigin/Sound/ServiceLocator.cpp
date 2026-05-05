#include "ServiceLocator.h"
std::unique_ptr<SoundSystem> ServiceLocator::m_SsInstance = std::make_unique<NullSoundSystem>();

void ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem> &&ss) {
    m_SsInstance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
}

void ServiceLocator::Shutdown() {
    m_SsInstance = std::make_unique<NullSoundSystem>();
}
