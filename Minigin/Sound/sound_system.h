#pragma once
#include <iostream>
#include "SDL3/SDL_log.h"
using sound_id = unsigned short;

class sound_system {
public:
    virtual ~sound_system() = default;
    virtual void play(sound_id id, float volume) = 0;
    virtual void play_music(sound_id id, float volume, bool loop) = 0;
};

class null_sound_system final : public sound_system {
public:
    void play(sound_id, float) override {}
    void play_music(sound_id, float, bool) override {};
};

class logging_sound_system final : public sound_system {
    std::unique_ptr<sound_system> _real_ss;

public:
    explicit logging_sound_system(std::unique_ptr<sound_system> &&ss) : _real_ss(std::move(ss)) {}

    void play(const sound_id id, const float volume) override {
        SDL_Log("Playing sound ID %i at volume %f", id, volume);

        _real_ss->play(id, volume);
    }

    void play_music(const sound_id id, const float volume, const bool loop) override {
        SDL_Log("Playing sound ID %i at volume %f", id, volume);

        _real_ss->play_music(id, volume,loop);
    }
};
