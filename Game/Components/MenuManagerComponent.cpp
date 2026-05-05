#include "MenuManagerComponent.h"
#include "Singletons/DeltaTime.h"

namespace dae {
    void MenuManagerComponent::Update() {
        m_AccTime += DeltaTime::GetInstance().Time();
        

    }
}
