#include "ScoreDisplayComponent.h"
using namespace dae;
void ScoreDisplayComponent::Render() const {
    assert(m_TextComponentRef != nullptr);
    m_TextComponentRef->SetText("Score: " + std::to_string(m_Score));
}
