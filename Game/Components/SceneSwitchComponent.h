#pragma once
#include "Components/Component.h"

namespace dae {
    class SceneSwitchComponent final : public Component {
        public:
        explicit SceneSwitchComponent(GameObject* Go) ;

        void Update() override;
        void Render() const override{}
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        void RemoveControlBindings();
        void AddControlBindings();
        bool m_Bound{false};

        //change state using class state machine to switch scenes
    };
}
