#pragma once
#include "Component.h"
#include <string>
#include <vector>

namespace ge {
    class TextComponent;
    class SpriteComponent;
    class TransformComponent;
    class Texture2D;

    class RenderComponent final : public Component {
    public:
        explicit RenderComponent(GameObject *go);
        RenderComponent(GameObject *go, const std::string &filename);
        void Update() override;
        void Render() const override;
        void SyncReferences();
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void InspectorGUI() override;
        void SetTexture(const std::string &filename);
        void SetTexture(const std::shared_ptr<Texture2D> &texture) { m_texture = texture; }
        static std::vector<std::string> GetTextureFiles(const std::string& directory, bool font = false);

    private:
        std::shared_ptr<Texture2D> m_texture;
        TransformComponent* m_transform{};
        SpriteComponent* m_sprite{};
        std::string m_FileName{};

        bool m_OverWritten{false};
    };
}
