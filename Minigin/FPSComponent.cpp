#include "FPSComponent.h"
#include <cassert>
#include "DeltaTime.h"
using namespace dae;

FPSComponent::FPSComponent(GameObject *go)
    : Component(go) {}

void FPSComponent::Update() {
    constexpr float waitTime{1.f};
    m_ElapsedTime += DeltaTime::GetInstance().Time();
    if (m_ElapsedTime >= waitTime) {
        //1/ms_per_frame
        m_FPS = 1.f / (DeltaTime::GetInstance().Time());
        m_ElapsedTime -= waitTime;
    }
}

void FPSComponent::Render() const {
    //FPS counter

    const float currentFPS{m_FPS};
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << currentFPS;

    assert(m_TextComponentRef != nullptr);
    m_TextComponentRef->SetText("FPS: " + ss.str());
}
