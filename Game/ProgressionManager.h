#pragma once
#include "Singletons/Singleton.h"

namespace game {
    class ProgressionManager final : public ge::Singleton<ProgressionManager> {
    public:
        // Delete copy and move operations to enforce Singleton property
        ProgressionManager(const ProgressionManager&) = delete;
        ProgressionManager(ProgressionManager&&) = delete;
        ProgressionManager& operator=(const ProgressionManager&) = delete;
        ProgressionManager& operator=(ProgressionManager&&) = delete;

        void Reset();

        int GetLives() const { return m_Lives; }
        void SetLives(int lives) { m_Lives = lives; }

        int GetScore() const { return m_Score; }
        void AddScore(int points) { m_Score += points; }

    private:
        friend Singleton<ProgressionManager>;

        ProgressionManager() = default;

        const int m_StartLives{ 3 };
        int m_Lives{ 3 };
        int m_Score{ 0 };
    };
}
