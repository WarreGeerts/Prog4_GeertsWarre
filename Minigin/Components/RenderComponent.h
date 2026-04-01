#pragma once
#include "Component.h"
#include <string>
#include <vector>

#include "SpriteComponent.h"

namespace dae {
    class TransformComponent;
    class Texture2D;

    class RenderComponent final : public Component {
    public:
        explicit RenderComponent(GameObject *go);
        RenderComponent(GameObject *go, const std::string &filename);
        void Update() override;
        void Render() const override;
        void InspectorGUI() override;
        void SetTexture(const std::string &filename);
        void SetTexture(const std::shared_ptr<Texture2D> &texture) { m_texture = texture; }
        void SetOverWriten(const bool overWriten) { m_OverWriten = overWriten; }

    private:
        std::shared_ptr<Texture2D> m_texture;
        TransformComponent* m_transform{};
        SpriteComponent* m_sprite{};
        std::string m_FileName{};
        bool m_expensiveLoaded{false};

        std::vector<std::string> GetTextureFiles(const std::string& directory);
        bool m_OverWriten{false};
    };
}
