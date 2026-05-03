#pragma once
#include <memory>
#include "sound_system.h"

class servicelocator final {
    static std::unique_ptr<sound_system> _ss_instance;

public:
    static sound_system &GetSoundSystem() { return *_ss_instance; }
    static void RegisterSoundSystem(std::unique_ptr<sound_system>&& ss) {
        _ss_instance = ss == nullptr ? std::make_unique<null_sound_system>() : std::move(ss);
    }
    static void Shutdown() {
        _ss_instance = std::make_unique<null_sound_system>();
    }
};
