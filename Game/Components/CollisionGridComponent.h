#pragma once
#include "Components/Component.h"

namespace game {
    class CollisionGridComponent final : public ge::Component {
    public:
        explicit CollisionGridComponent(ge::GameObject *go);
        void Update() override;
        void Render() const override;
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

    private:
        std::vector<int> m_GridData;

        int m_Columns{1};
        int m_Rows{1};

        float m_Scale{1};
        bool m_DebugLines{false};
        int m_CurrentBrush{0};

        int GetIndex(int x, int y) const;
        void ResizeGrid();
    };
}
