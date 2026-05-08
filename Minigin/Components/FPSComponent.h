#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace ge {
    class FPSComponent final : public Component {
    public:
        explicit FPSComponent(GameObject *go);
        void Update() override;
        void Render() const override;
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void SetRefTextComponent(TextComponent *component) {m_TextComponentRef = component; }

    private:
        float m_FPS{0.f};
        int m_Frames{0};
        float m_ElapsedTime{0.f};
        float m_TotalTime{0.f};
        TextComponent *m_TextComponentRef{};
        bool onLoad{false};
    };
}
