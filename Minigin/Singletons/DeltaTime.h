#pragma once
#include <chrono>
#include <iostream>
#include "Singleton.h"

namespace ge {
    class DeltaTime final : public Singleton<DeltaTime> {
    public:
        void StartDeltaTime();
        void ReCalcLag();
        [[nodiscard]] auto CalcSleepTime() const{
            return m_CurrentTime + std::chrono::milliseconds(static_cast<int>(m_Ms_Per_Frame * 1000)) - Clock::now();
        }
        [[nodiscard]] float Time() const { return m_DeltaTime; }
        [[nodiscard]] float FixedTime() const { return m_FixedDeltaTime; }
        [[nodiscard]] float Lag() const { return m_Lag; }
        [[nodiscard]] float GetMsPerFrame() const { return m_Ms_Per_Frame; }
        void SetFPS(int fps);
        void Reset();

    private:
        friend Singleton<DeltaTime>;
        using Clock = std::chrono::high_resolution_clock;
        Clock::time_point m_LastTime{Clock::now()};
        Clock::time_point m_CurrentTime{};
        int m_FPS{60};
        float m_Ms_Per_Frame{1.f / static_cast<float>(m_FPS)};
        float m_DeltaTime{0.0f};
        float m_FixedDeltaTime{0.02f};
        float m_Lag{0.0f};
    };
}
