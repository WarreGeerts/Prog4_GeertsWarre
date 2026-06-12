#pragma once
#include "Updatable.h"

namespace ge {
    class EngineUpdater {
        static inline Updatable* m_Updater = nullptr;
    public:
        static void SetUpdater(Updatable* updater) { m_Updater = updater; }
        static void Update() { if(m_Updater) m_Updater->Update(); }
    };
}
