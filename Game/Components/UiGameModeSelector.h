#pragma once
#include <SDL3/SDL_pixels.h>
#include "Components/Component.h"
#include "Singletons/EventManager.h"

namespace game {
    class UiGameModeSelector final : public ge::Component {
    public:
        explicit UiGameModeSelector(ge::GameObject *go);
        void Update() override;
        void Render() const override {};
        void InspectorGUI() override;
        [[nodiscard]] nlohmann::ordered_json Serialize() const override;
        void Deserialize(const nlohmann::ordered_json &data) override;

        void AddHandles();

    private:
        SDL_Color m_ColorDefault{255, 255, 255, 255};
        SDL_Color m_ColorSelected{255, 0, 0, 255};
        void SwitchSelection();
        void SelectionDown();
        void SelectionUp();

        void MakeChoice() const;

        int m_CurrentOption{0};
        int m_PrevOption{0};

        bool m_LateLoad{false};

        ge::EventHandle m_HandleUp;
        ge::EventHandle m_HandleDown;
        ge::EventHandle m_HandleSelected;
    };
}
