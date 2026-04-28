#pragma once
#include <condition_variable>
#include <memory>
#include <queue>
#include <thread>
#include <mutex>
#include "sound_system.h"

class sdl_sound_system final : public sound_system {
public:
    sdl_sound_system();
    ~sdl_sound_system() override;

    sdl_sound_system(const sdl_sound_system& other) = delete;
    sdl_sound_system(sdl_sound_system&& other) = delete;
    sdl_sound_system& operator=(const sdl_sound_system& other) = delete;
    sdl_sound_system& operator=(sdl_sound_system&& other) = delete;

    void play(sound_id id, float volume) override;
    void play_music(sound_id id, float volume, bool loop) override;

private:
    struct SoundRequest {
        sound_id id;
        float volume;
        bool is_music;
        bool loop;
    };

    struct Impl;
    std::unique_ptr<Impl> pImpl;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::queue<SoundRequest> _queue;
    bool _running;

    std::thread _audio_thread;

    void process_queue();
};