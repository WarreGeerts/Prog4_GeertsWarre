#pragma once
#include <memory>
#include "SoundSystem.h"

namespace ge {
    class SdlSoundSystem final : public SoundSystem {
    public:
        SdlSoundSystem();
        ~SdlSoundSystem() override;

        // Standard Rule of 5 boilerplate
        SdlSoundSystem(const SdlSoundSystem& other) = delete;
        SdlSoundSystem(SdlSoundSystem&& other) = delete;
        SdlSoundSystem& operator=(const SdlSoundSystem& other) = delete;
        SdlSoundSystem& operator=(SdlSoundSystem&& other) = delete;

        void Play(sound_id id, float volume) override;
        void PlayMusic(sound_id id, float volume, bool loop) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };
}