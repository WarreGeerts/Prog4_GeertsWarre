#include "GSGame.h"
#include "SDL3/SDL_log.h"
#include "Singletons/DeltaTime.h"
#include "Singletons/SceneManager.h"
#include "Sound/ServiceLocator.h"

namespace game {
    void GSGame::OnEnter() {
        auto &ss = ge::ServiceLocator::GetSoundSystem();
        ss.Play(1, 0.5f);
        m_WaitTimer = 3.f;
        m_MusicStarted = false;
        m_CurrentLevelIndex = ge::SceneManager::GetInstance().GetCurrentSceneIdx();
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

    void GSGame::NextLevel() {
        m_CurrentLevelIndex++;
        if (m_CurrentLevelIndex > m_HighestLevelIndex) {
            m_CurrentLevelIndex = m_LowestLevelIndex;
        }
        SDL_Log("new level");
        ge::SceneManager::GetInstance().SelectSceneByIndex(m_CurrentLevelIndex);
    }
}
