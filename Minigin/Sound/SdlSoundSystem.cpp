#include "SdlSoundSystem.h"
#include <SDL3/SDL.h>
#include <map>
#include <ranges>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
namespace ge {
    struct SdlSoundSystem::Impl {
        struct SoundData {
            uint8_t *buffer;
            uint32_t length;
            SDL_AudioSpec spec;
        };

        struct SoundRequest {
            sound_id id;
            float volume;
            bool is_music;
            bool loop;
        };

        enum class SoundID : sound_id {
            BGM = 0,
            CoinPickup = 1,
            Death = 2
        };

        std::map<sound_id, SoundData> sounds;
        SDL_AudioStream *musicStream{nullptr};
        sound_id currentMusicId{0};
        bool isLooping{false};
        std::mutex mutex;
        std::queue<SoundRequest> queue;
        bool running{true};
#ifndef __EMSCRIPTEN__
        std::condition_variable CV;
        std::thread audioThread;
#endif
        Impl() {
#ifndef __EMSCRIPTEN__
            audioThread = std::thread(&Impl::ProcessQueue, this);
#endif
        }

        ~Impl() {
            {
                std::lock_guard<std::mutex> lock(mutex);
                running = false;
            }
#ifndef __EMSCRIPTEN__
            CV.notify_all();

            if (audioThread.joinable()) {
                audioThread.join();
            }
#endif

            if (musicStream) {
                SDL_DestroyAudioStream(musicStream);
            }
            for (const auto &val: sounds | std::views::values) {
                SDL_free(val.buffer);
            }
        }

        static void SDLCALL MusicCallback(void *userdata, SDL_AudioStream *stream, int additional_amount,
                                          int total_amount) {
            auto *impl = static_cast<SdlSoundSystem::Impl *>(userdata);
            impl->RefillMusic(stream);
        }

        void RefillMusic(SDL_AudioStream *stream) {
            std::lock_guard<std::mutex> lock(mutex);
            if (isLooping && sounds.contains(currentMusicId)) {
                const auto &data = sounds[currentMusicId];
                SDL_PutAudioStreamData(stream, data.buffer, static_cast<int>(data.length));
            }
        }

        void ProcessQueueOnce() {
            while (true) {
                SoundRequest request{};
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    if (queue.empty()) return;
                    request = queue.front();
                    queue.pop();
                }

                if (request.is_music) {
                    PlayMusicInternal(request.id, request.volume, request.loop);
                } else {
                    PlaySoundInternal(request.id, request.volume);
                }
            }
        }

#ifndef __EMSCRIPTEN__
        void ProcessQueue() {
            while (true) {
                std::unique_lock<std::mutex> lock(mutex);
                CV.wait(lock, [this] { return !queue.empty() || !running; });
                if (!running && queue.empty()) return;
                lock.unlock();
                ProcessQueueOnce();
            }
        }
#endif

        void PlaySoundInternal(const sound_id id, const float volume) {
            if (!sounds.contains(id)) Load(id);
            const auto &[buffer, length, spec] = sounds[id];

            SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(
                SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
            if (stream) {
                SDL_SetAudioStreamGain(stream, volume);
                SDL_PutAudioStreamData(stream, buffer, static_cast<int>(length));
                SDL_ResumeAudioStreamDevice(stream);
            }
        }

        void PlayMusicInternal(const sound_id id, const float volume, const bool loop) {
            if (!sounds.contains(id)) Load(id);
            const auto &[buffer, length, spec] = sounds[id];

            if (musicStream) {
                SDL_DestroyAudioStream(musicStream);
            }

            musicStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, MusicCallback, this);

            if (musicStream) {
                currentMusicId = id;
                isLooping = loop;
                SDL_SetAudioStreamGain(musicStream, volume);

                SDL_PutAudioStreamData(musicStream, buffer, static_cast<int>(length));
                SDL_ResumeAudioStreamDevice(musicStream);
            }
        }

        void UpdateMusicLoop() {
            if (musicStream && isLooping) {
                if (SDL_GetAudioStreamQueued(musicStream) < 4096) {
                    const auto &data = sounds[currentMusicId];
                    SDL_PutAudioStreamData(musicStream, data.buffer, static_cast<int>(data.length));
                }
            }
        }

        void Load(sound_id id) {
            std::string filename;
            switch (static_cast<SoundID>(id)) {
                case SoundID::BGM: filename = "BGM.wav";
                    break;
                case SoundID::CoinPickup: filename = "Coin.wav";
                    break;
                case SoundID::Death: filename = "Death.wav";
                    break;
            }

            const std::string path = "Data/Sounds/" + filename;
            SoundData data{};
            if (SDL_LoadWAV(path.c_str(), &data.spec, &data.buffer, &data.length)) {
                sounds[id] = data;
                SDL_Log("Successfully loaded: %s", path.c_str());
            } else {
                SDL_Log("SDL_LoadWAV failed: %s", SDL_GetError());
            }
        }
    };

    SdlSoundSystem::SdlSoundSystem()
        : pImpl(std::make_unique<Impl>()) {}

    SdlSoundSystem::~SdlSoundSystem() = default;

    void SdlSoundSystem::Play(const sound_id id, const float volume) {
        std::lock_guard<std::mutex> lock(pImpl->mutex);
        pImpl->queue.push({id, volume, false, false});
#ifndef __EMSCRIPTEN__
        pImpl->CV.notify_one();
#endif
        }

    void SdlSoundSystem::PlayMusic(const sound_id id, const float volume, const bool loop) {
        std::lock_guard<std::mutex> lock(pImpl->mutex);
        pImpl->queue.push({id, volume, true, loop});
#ifndef __EMSCRIPTEN__
        pImpl->CV.notify_one();
#endif
        }

    void SdlSoundSystem::Update() {
#ifdef __EMSCRIPTEN__
        pImpl->ProcessQueueOnce();
        pImpl->UpdateMusicLoop();
#endif
    }
}
