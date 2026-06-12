#include "SoundSystem.h"
#include "SDL3/SDL_log.h"
namespace ge {
    void LoggingSoundSystem::Play(const sound_id id, const float volume) {
        SDL_Log("Playing sound ID %i at volume %f", id, volume);
        m_RealSs->Play(id, volume);
    }

    void LoggingSoundSystem::PlayMusic(const sound_id id, const float volume, const bool loop) {
        SDL_Log("Playing sound ID %i at volume %f", id, volume);
        m_RealSs->PlayMusic(id, volume, loop);
    }

    void LoggingSoundSystem::Update() {
        m_RealSs->Update();
    }

    void LoggingSoundSystem::Load(const sound_id id, const std::string &path) {
        SDL_Log("LoggingSoundSystem: Load requested for %s", path.c_str());
        m_RealSs->Load(id, path);
    }}
