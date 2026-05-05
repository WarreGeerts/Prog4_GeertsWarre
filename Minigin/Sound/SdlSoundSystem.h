#pragma once
#include <condition_variable>
#include <memory>
#include <queue>
#include <thread>
#include <mutex>
#include "SoundSystem.h"

class SdlSoundSystem final : public SoundSystem {
public:
    SdlSoundSystem();
    ~SdlSoundSystem() override;

    SdlSoundSystem(const SdlSoundSystem& other) = delete;
    SdlSoundSystem(SdlSoundSystem&& other) = delete;
    SdlSoundSystem& operator=(const SdlSoundSystem& other) = delete;
    SdlSoundSystem& operator=(SdlSoundSystem&& other) = delete;

    void Play(sound_id id, float volume) override;
    void PlayMusic(sound_id id, float volume, bool loop) override;

private:
    struct SoundRequest {
        sound_id id;
        float volume;
        bool is_music;
        bool loop;
    };

    struct Impl;
    std::unique_ptr<Impl> pImpl;

    std::mutex m_Mutex;
    std::condition_variable m_CV;
    std::queue<SoundRequest> m_Queue;
    bool m_Running;

    std::thread m_AudioThread;

    void ProcessQueue();
};