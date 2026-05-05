#pragma once
#include <memory>
#include "SoundSystem.h"

class ServiceLocator final {
    static std::unique_ptr<SoundSystem> m_SsInstance;

public:
    static SoundSystem &GetSoundSystem() { return *m_SsInstance; }
    static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss) ;
    static void Shutdown();
};
