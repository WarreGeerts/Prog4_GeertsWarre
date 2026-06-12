#include "GSGame.h"
#include "SDL3/SDL_log.h"
#include "Singletons/DeltaTime.h"
#include "Sound/ServiceLocator.h"

namespace game {
    void GSGame::OnEnter() {
        auto &ss = ge::ServiceLocator::GetSoundSystem();
        ss.Play(1, 0.5f);
        m_WaitTimer = 3.f;
        m_MusicStarted = false;
    }

    void GSGame::Update() {
        if (!m_MusicStarted && m_WaitTimer > 0.0f) {
            m_WaitTimer -= ge::DeltaTime::GetInstance().Time();
            if (m_WaitTimer <= 0.0f) {
                ge::ServiceLocator::GetSoundSystem().PlayMusic(0, 0.5f, true);
                m_MusicStarted = true;
            }
        }
    }
}
