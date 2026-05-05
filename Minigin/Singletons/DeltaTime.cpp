#include "DeltaTime.h"

void dae::DeltaTime::StartDeltaTime() {
    m_CurrentTime = Clock::now();
    m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_LastTime).count();
    m_LastTime = m_CurrentTime;
    m_Lag += m_DeltaTime;
}

void dae::DeltaTime::ReCalcLag() {
    m_Lag -= m_FixedDeltaTime;
}

auto dae::DeltaTime::CalcSleepTime() const {
    return m_CurrentTime + std::chrono::milliseconds(static_cast<int>(m_Ms_Per_Frame * 1000)) - Clock::now();
}

void dae::DeltaTime::SetFPS(const int fps) {
    m_FPS = fps;
    m_Ms_Per_Frame = 1.f / static_cast<float>(m_FPS);
    std::cout << "ms per frame: " << m_Ms_Per_Frame << std::endl;
}
