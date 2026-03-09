#pragma once
#include "Component.h"
#include <string>

namespace dae {
    class Texture2D;

    class RenderComponent final : public Component {
    public:
        explicit RenderComponent(GameObject *go);
        RenderComponent(GameObject *go, const std::string &filename);
        void Update() override;
        void Render() const override;
        void RenderGUI() override{};
        void SetTexture(const std::string &filename);
        void SetTexture(const std::shared_ptr<Texture2D> &texture) { m_texture = texture; }

    private:
        std::shared_ptr<Texture2D> m_texture;
    };
}
