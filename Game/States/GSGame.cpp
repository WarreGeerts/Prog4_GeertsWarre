#include "GSGame.h"

#include <SDL3/SDL_scancode.h>

#include "Commands/MuteCommand.h"
#include "Commands/SkipLevelCommand.h"
#include "Input/InputManager.h"
#include "SDL3/SDL_log.h"
#include "Singletons/DeltaTime.h"
#include "Singletons/EventManager.h"
#include "Singletons/SceneManager.h"
#include "Singletons/RealTimeEditor/SceneSerializer.h"
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
        if (m_IsTransitioning) return;

        if (m_TransitionRequested) {
            m_IsTransitioning = true;
            PerformLevelTransition();
            m_IsTransitioning = false;
        }

        if (!m_MusicStarted && m_WaitTimer > 0.0f) {
            m_WaitTimer -= ge::DeltaTime::GetInstance().Time();
            if (m_WaitTimer <= 0.0f) {
                ge::ServiceLocator::GetSoundSystem().PlayMusic(0, 0.5f, true);
                m_MusicStarted = true;
            }
        }
    }

    void GSGame::NextLevel() {
        m_TransitionRequested = true;
    }

    void GSGame::PerformLevelTransition() {
        m_TransitionRequested = false;

        ge::EventManager::GetInstance().ClearAllEvents();
        ge::InputManager::GetInstance().ClearBindings();
        RegisterDebugBindings();

        auto& sceneManager = ge::SceneManager::GetInstance();

        auto& oldScene = sceneManager.GetSceneByIdx(m_CurrentLevelIndex);
        oldScene.RemoveAll();

        m_CurrentLevelIndex++;
        if (m_CurrentLevelIndex > m_HighestLevelIndex) {
            m_CurrentLevelIndex = m_LowestLevelIndex;
        }

        auto& nextScene = sceneManager.GetSceneByIdx(m_CurrentLevelIndex);
        nextScene.RemoveAll();

        std::string filename = "Level_" + std::to_string(m_CurrentLevelIndex) + ".json";
        const std::filesystem::path path = std::filesystem::current_path() / "Data" / "Scenes" / filename;
        ge::SceneSerializer::LoadScene(path.string(), nextScene);

        sceneManager.SelectSceneByIndex(m_CurrentLevelIndex);

        ge::DeltaTime::GetInstance().Reset();
    }

    void  GSGame::RegisterDebugBindings() {
        auto &Input{ge::InputManager::GetInstance()};
        Input.AddBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_F1), true},
                             std::make_unique<SkipLevelCommand>());
        Input.AddBinding({ge::KeyState::Down, static_cast<int>(SDL_SCANCODE_F2), true},
                             std::make_unique<MuteCommand>());
    }
}
