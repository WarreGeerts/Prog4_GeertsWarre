#include "LivesDisplayComponent.h"
#include <cassert>

using namespace dae;

void LivesDisplayComponent::Render() const {
    assert(m_TextComponentRef != nullptr);
    m_TextComponentRef->SetText("# lives : " + std::to_string(m_Lives));
}
