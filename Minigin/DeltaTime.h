#pragma once
#include <chrono>
#include "Singleton.h"

namespace dae {
    class DeltaTime final : public Singleton<DeltaTime> {
    public:
        void Update() {

            const auto current_time = std::chrono::high_resolution_clock::now();
            m_DeltaTime = std::chrono::duration<float>(current_time - m_LastTime).count();
            m_LastTime = current_time;

            const auto target_time{m_LastTime + std::chrono::milliseconds(m_Ms_Per_Frame)};
            m_SleepTime = target_time - std::chrono::high_resolution_clock::now();
        }

        float Time() const { return m_DeltaTime; }
        auto SleepTime() const { return m_SleepTime; }

    private:
        friend Singleton<DeltaTime>;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime{
            std::chrono::high_resolution_clock::now()
        };
        std::chrono::nanoseconds m_SleepTime{};
        const int m_Ms_Per_Frame{17};
        float m_DeltaTime{0.0f};
    };
}
