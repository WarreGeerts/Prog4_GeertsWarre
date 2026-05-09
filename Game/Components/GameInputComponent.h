#pragma once
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace game {
    class GameInputComponent final : public ge::Component {
    public:
        explicit GameInputComponent(ge::GameObject *Go);
        ~GameInputComponent() override = default;
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        void AddControlBindings();
        void RemoveControlBindings();
        [[nodiscard]] nlohmann::ordered_json Serialize() const override {return nlohmann::ordered_json();};
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        bool m_Bound{false};
    };
}
