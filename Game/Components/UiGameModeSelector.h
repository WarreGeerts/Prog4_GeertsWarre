#pragma once
#include "Components/Component.h"

namespace dae {
    class UiGameModeSelector final : public Component {
    public:
        explicit UiGameModeSelector (GameObject* go) : Component(go, "UiGameModeSelector") {};
        void Update() override;
        void Render() const override;
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
    };
}
