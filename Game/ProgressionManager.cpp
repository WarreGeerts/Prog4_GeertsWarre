#include "ProgressionManager.h"

namespace game {
    void ProgressionManager::Reset() {
        m_Lives = m_StartLives;
        m_Score = 0;
    }
}