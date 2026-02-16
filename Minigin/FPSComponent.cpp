#include "FPSComponent.h"
#include "DeltaTime.h"
using namespace dae;

FPSComponent::FPSComponent(GameObject *go)
    : Component(go) {}

void FPSComponent::Update() {
    const float dt = DeltaTime::GetInstance().Time();

    m_Time += dt;
    m_Frames++;
    m_IntervalTime += dt;
    m_IntervalFrames++;

    // Update FPS every 0.2 seconds (5x/sec)
    if (m_IntervalTime >= m_UpdateInterval) {
        m_FPS = static_cast<int>(static_cast<float>(m_IntervalFrames) / m_IntervalTime);
        m_IntervalTime -= m_UpdateInterval;  // Carry remainder
        m_IntervalFrames = 0;
    }
}
