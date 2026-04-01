#include "FPSComponent.h"
#include <cassert>
#include "Singletons/DeltaTime.h"
#include <iomanip>

using namespace dae;

FPSComponent::FPSComponent(GameObject *go)
    : Component(go, "FPSComponent") {}

void FPSComponent::Update() {
    m_Frames++;
    m_TotalTime += DeltaTime::GetInstance().Time();
    m_ElapsedTime += DeltaTime::GetInstance().Time();

    if (m_ElapsedTime >= 1.f) {
        m_FPS = static_cast<float>(m_Frames) / m_ElapsedTime;
        m_ElapsedTime = 0.f;
        m_Frames = 0;
    }
}

void FPSComponent::Render() const {
    const float currentFPS{m_FPS};
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << currentFPS;

    assert(m_TextComponentRef != nullptr);
    m_TextComponentRef->SetText("FPS: " + ss.str());
}
