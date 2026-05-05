#pragma once
#include <memory>
using sound_id = unsigned short;

class SoundSystem {
public:
    virtual ~SoundSystem() = default;
    virtual void Play(sound_id id, float volume) = 0;
    virtual void PlayMusic(sound_id id, float volume, bool loop) = 0;
};

class NullSoundSystem final : public SoundSystem {
public:
    void Play(sound_id, float) override {}
    void PlayMusic(sound_id, float, bool) override {};
};

class LoggingSoundSystem final : public SoundSystem {
    std::unique_ptr<SoundSystem> m_RealSs;

public:
    explicit LoggingSoundSystem(std::unique_ptr<SoundSystem> &&ss) : m_RealSs(std::move(ss)) {}
    void Play(sound_id id, float volume) override;
    void PlayMusic(sound_id id, float volume, bool loop) override;
};
