#pragma once
#include <chrono>
#include "Singleton.h"
#include <iostream>

namespace dae {
    class DeltaTime final : public Singleton<DeltaTime> {
    public:
        void StartDeltaTime() {
            m_CurrentTime = std::chrono::high_resolution_clock::now();
            m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_LastTime).count();
            m_LastTime = m_CurrentTime;
            m_Lag += m_DeltaTime;
        }

        void ReCalcLag() {
            m_Lag -= m_FixedDeltaTime;
        }

        auto CalcSleepTime() const {
            return
                    m_CurrentTime + std::chrono::milliseconds(static_cast<int>(m_Ms_Per_Frame * 1000)) -
                    std::chrono::high_resolution_clock::now();
        }

        float Time() const { return m_DeltaTime; }
        float FixedTime() const { return m_FixedDeltaTime; }
        float Lag() const { return m_Lag; }

        void SetFPS(const int fps) {
            m_FPS = fps;
            m_Ms_Per_Frame = 1.f / static_cast<float>(m_FPS);
            std::cout << "ms per frame: " << m_Ms_Per_Frame << std::endl;
        }

    private:
        friend Singleton<DeltaTime>;
        std::chrono::time_point<std::chrono::system_clock> m_LastTime{std::chrono::high_resolution_clock::now()};
        std::chrono::time_point<std::chrono::system_clock> m_CurrentTime{};
        int m_FPS{60};
        float m_Ms_Per_Frame{1.f / static_cast<float>(m_FPS)};
        float m_DeltaTime{0.0f};
        float m_FixedDeltaTime{0.02f};
        float m_Lag{0.0f};
    };
}
