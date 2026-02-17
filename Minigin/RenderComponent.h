#pragma once
#include "Component.h"
namespace dae {
    class Texture2D;
    class RenderComponent final : public Component {
    public:
        RenderComponent(GameObject* go);
        RenderComponent(GameObject* go, const std::string& filename);

        void Update() override;

        void Render() const override;
        void SetTexture(const std::string &filename);

    private:
        std::shared_ptr<Texture2D> m_texture;
    };
}