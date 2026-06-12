#pragma once
#include <vector>
#include "HamburgerHolderComponent.h"
#include "Components/Component.h"

namespace game {
    class HamburgerManager final : public ge::Component {
    public:
        explicit HamburgerManager(ge::GameObject *go) : Component(go, "HamburgerManager") {};
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;
        void RegisterPlate(HamburgerHolderComponent *plate);
        [[nodiscard]] bool IsLevelComplete() const;

    private:
        std::vector<HamburgerHolderComponent *> m_Plates;
        int m_RequiredPartsPerPlate{4};
        bool m_Finished{false};
    };
}
