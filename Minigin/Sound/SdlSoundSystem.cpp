#include "SdlSoundSystem.h"
#include <SDL3/SDL.h>
#include <map>
#include <ranges>
#include <string>

struct SdlSoundSystem::Impl {
    struct SoundData {
        uint8_t *buffer;
        uint32_t length;
        SDL_AudioSpec spec;
    };

    std::map<sound_id, SoundData> _sounds;
    SDL_AudioStream* _music_stream{nullptr};
    sound_id _current_music_id{0};
    bool _is_looping{false};

    void PlaySound(const sound_id id, const float volume) {
        if (!_sounds.contains(id)) {
            Load(id);
        }

        const auto &[buffer, length, spec] = _sounds[id];

        SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
        if (stream) {
            SDL_SetAudioStreamGain(stream, volume);

            SDL_PutAudioStreamData(stream, buffer, static_cast<int>(length));
            SDL_ResumeAudioStreamDevice(stream);
        }
    }

    void PlayMusic(const sound_id id, const float volume, bool loop) {
        if (!_sounds.contains(id)) Load(id);
        const auto &[buffer, length, spec] = _sounds[id];

        if (_music_stream) SDL_DestroyAudioStream(_music_stream);

        _music_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);

        if (_music_stream) {
            _current_music_id = id;
            _is_looping = loop; // Use the bool to set the internal state

            SDL_SetAudioStreamGain(_music_stream, volume);
            SDL_PutAudioStreamData(_music_stream, buffer, static_cast<int>(length));
            SDL_ResumeAudioStreamDevice(_music_stream);
        }
    }

    void UpdateMusicLoop() {
        // Only refill if the user explicitly set 'loop' to true
        if (_music_stream && _is_looping) {
            // If less than a tiny bit of audio is left, push the data again
            if (SDL_GetAudioStreamQueued(_music_stream) < 4096) {
                const auto &data = _sounds[_current_music_id];
                SDL_PutAudioStreamData(_music_stream, data.buffer, static_cast<int>(data.length));
            }
        }
    }

    enum class SoundID : sound_id {
        BGM = 0,
        CoinPickup = 1,
        Death = 2
    };

    void Load(const sound_id id) {
        std::string filename;

        switch (static_cast<SoundID>(id)) {
            case SoundID::BGM: filename = "BGM.wav";
                break;
            case SoundID::CoinPickup: filename = "Coin.wav";
                break;
            case SoundID::Death: filename = "Death.wav";
                break;
            default: filename = "default.wav";
                break;
        }

        const std::string path = "Data/Sounds/" + filename;

        SoundData data{};
        if (SDL_LoadWAV(path.c_str(), &data.spec, &data.buffer, &data.length)) {
            _sounds[id] = data;
            SDL_Log("Successfully loaded: %s", path.c_str());
        } else {
            SDL_Log("SDL_LoadWAV failed, error: %s", SDL_GetError());
        }
    }

    ~Impl() {
        if (_music_stream) SDL_DestroyAudioStream(_music_stream);
        for (const auto &val: _sounds | std::views::values) {
            SDL_free(val.buffer);
        }
    }
};

SdlSoundSystem::SdlSoundSystem()
    : pImpl(std::make_unique<Impl>())
      , m_Running(true) {
    m_AudioThread = std::thread(&SdlSoundSystem::ProcessQueue, this);
}

SdlSoundSystem::~SdlSoundSystem() {
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Running = false;
    }
    m_CV.notify_all();
    if (m_AudioThread.joinable()) {
        m_AudioThread.join();
    }
}

void SdlSoundSystem::Play(const sound_id id, const float volume) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Queue.push({id, volume, false, false});
    m_CV.notify_one();
}

void SdlSoundSystem::PlayMusic(const sound_id id, const float volume, const bool loop) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Queue.push({id, volume, true, loop});
    m_CV.notify_one();
}

void SdlSoundSystem::ProcessQueue() {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(m_Mutex);

            const bool got_request = m_CV.wait_for(lock, std::chrono::milliseconds(100), [this] {
                return !m_Queue.empty() || !m_Running;
            });

            if (!m_Running && m_Queue.empty()) return;

            if (got_request) {
                SoundRequest request{};
                request = m_Queue.front();
                m_Queue.pop();

                if (request.is_music) pImpl->PlayMusic(request.id, request.volume, request.loop);
                else pImpl->PlaySound(request.id, request.volume);
            }
        }

        pImpl->UpdateMusicLoop();
    }
}
