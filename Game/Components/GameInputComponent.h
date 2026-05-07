#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace dae {
    class GameInputComponent final : public Component {
    public:
        explicit GameInputComponent(GameObject *Go);
        ~GameInputComponent() override = default;
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        void AddControlBindings();
        void RemoveControlBindings();
        [[nodiscard]] nlohmann::ordered_json Serialize() const override { return {}; }
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        bool m_Bound{false};
        EventHandle m_Handle;
    };
}
