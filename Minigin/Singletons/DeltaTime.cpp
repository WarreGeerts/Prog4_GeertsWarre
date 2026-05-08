#include "DeltaTime.h"

namespace ge {
    auto DeltaTime::StartDeltaTime() -> void {
        m_CurrentTime = Clock::now();
        m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_LastTime).count();
        m_LastTime = m_CurrentTime;
        m_Lag += m_DeltaTime;
    }

    void DeltaTime::ReCalcLag() {
        m_Lag -= m_FixedDeltaTime;
    }

    void DeltaTime::SetFPS(const int fps) {
        m_FPS = fps;
        m_Ms_Per_Frame = 1.f / static_cast<float>(m_FPS);
        std::cout << "ms per frame: " << m_Ms_Per_Frame << std::endl;
    }
}
