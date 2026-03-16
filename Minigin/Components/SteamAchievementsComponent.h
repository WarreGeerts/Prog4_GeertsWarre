#pragma once
#include "Component.h"

#if USE_STEAMWORKS
#pragma warning(push)
#pragma warning(disable:4996)
#include <steam_api.h>
#pragma warning(pop)
#endif

#include "Singletons/EventManager.h"

namespace dae {
    class SteamAchievementsComponent final : public Component {
    public:
        explicit SteamAchievementsComponent(GameObject *go) : Component(go) {
            EventManager::GetInstance().AttachEvent(
                ACH_WIN_ONE_GAME, [this](const Event &) { UnlockAchievement(); });

#if USE_STEAMWORKS
            SteamUserStats()->RequestUserStats(SteamUser()->GetSteamID());
#endif
        }

        void Update() override {};
        void Render() const override{};
        void RenderGUI() override{};

    private:
        void UnlockAchievement() const {
#if USE_STEAMWORKS
            bool wasUnlocked;
            SteamUserStats()->GetAchievement("ACH_WIN_ONE_GAME", &wasUnlocked);
            if (!wasUnlocked) {
                SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
                SteamUserStats()->StoreStats();
            }
#endif
        }
    };
}
