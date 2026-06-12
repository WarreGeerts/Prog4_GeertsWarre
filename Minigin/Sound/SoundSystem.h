#pragma once
#include <memory>
using sound_id = unsigned short;

namespace ge {
    class SoundSystem {
    public:
        virtual ~SoundSystem() = default;
        virtual void Play(sound_id id, float volume) = 0;
        virtual void PlayMusic(sound_id id, float volume, bool loop) = 0;
        virtual void Update() = 0;
        virtual void Load(sound_id id, const std::string& path) = 0;
        virtual void SetMasterVolume(float volume) = 0;
    };

    class NullSoundSystem final : public SoundSystem {
    public:
        void Play(sound_id, float) override {}
        void PlayMusic(sound_id, float, bool) override {};
        void Update() override {};
        void Load(sound_id, const std::string&) override {};
        void SetMasterVolume(float) override{};
    };

    class LoggingSoundSystem final : public SoundSystem {
        std::unique_ptr<SoundSystem> m_RealSs;

    public:
        explicit LoggingSoundSystem(std::unique_ptr<SoundSystem> &&ss) : m_RealSs(std::move(ss)) {}
        void Play(sound_id id, float volume) override;
        void PlayMusic(sound_id id, float volume, bool loop) override;
        void Update() override;
        void Load(sound_id, const std::string&) override;
        void SetMasterVolume(float) override{};
    };
}
