#include "Component.h"
#include "GameObject.h"

using namespace dae;

bool Component::NeedsUpdate() {
    if (m_LastSeenVersion != m_gameObject->GetComponentListVersion()) {
        m_LastSeenVersion = m_gameObject->GetComponentListVersion();
        return true;
    }
    return false;
}
