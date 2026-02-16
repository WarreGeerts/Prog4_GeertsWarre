#include "FPSComponent.h"
#include "ResourceManager.h"
#include "Font.h"
using namespace dae;

FPSComponent::FPSComponent(GameObject* go)
    : Component(go) {
}

void FPSComponent::Update() {
    m_time += 0; //deltatime
    m_frames++;
    
    if (m_time >= 1.0f) {
        m_fps = static_cast<int>(static_cast<float>(m_frames) / m_time);
        m_text = "FPS: " + std::to_string(m_fps);
        m_time = 0.0f;
        m_frames = 0;
    }
}