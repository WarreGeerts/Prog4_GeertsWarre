#include "SpriteComponent.h"
#include <algorithm>
#include "Singletons/ResourceManager.h"
#include "Texture2D.h"
#include <SDL3/SDL_render.h>
using namespace dae;

SpriteComponent::SpriteComponent(GameObject *go, const std::string &spritesheetPath, const SpriteFraming &spriteFrame,
                                 const int frameIdx)
    : Component(go, "SpriteComponent"),
      m_Columns{spriteFrame.columns}, m_Rows{spriteFrame.rows}, m_FrameWidth{spriteFrame.frameWidth},
      m_FrameHeight{spriteFrame.frameHeight}, m_SpacingX{spriteFrame.spacingX}, m_SpacingY{spriteFrame.spacingY} {
    SetSpritesheetPath(spritesheetPath);
    m_DstRect = {0.f, 0.f, m_FrameWidth, m_FrameHeight};
    SetFrame(frameIdx);
}

void SpriteComponent::SetSpritesheetPath(const std::string &spritesheetPath) {
    m_Spritesheet = ResourceManager::GetInstance().LoadTexture(spritesheetPath);
    if (m_Spritesheet) {
        const auto size{m_Spritesheet->GetSize()};
        m_SrcRect = {0.f, 0.f, size.x, size.y};
        SDL_SetTextureScaleMode(m_Spritesheet->GetSDLTexture(), SDL_SCALEMODE_PIXELART);
    }
}

void SpriteComponent::SetFrame(int frameIndex) {
    frameIndex = std::clamp(frameIndex, 0, m_Columns * m_Rows - 1);

    const int col{frameIndex % m_Columns};
    const int row{frameIndex / m_Columns};

    m_SrcRect.x = static_cast<float>(col) * (m_FrameWidth + m_SpacingX);
    m_SrcRect.y = static_cast<float>(row) * (m_FrameHeight + m_SpacingY);
    m_SrcRect.w = m_FrameWidth;
    m_SrcRect.h = m_FrameHeight;

    m_DstRect.w = m_FrameWidth * m_ScaleX;
    m_DstRect.h = m_FrameHeight * m_ScaleY;
}

void SpriteComponent::SetScale(const float scaleX, const float scaleY) {
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
    m_DstRect.w = m_FrameWidth * m_ScaleX;
    m_DstRect.h = m_FrameHeight * m_ScaleY;
}
