#pragma once
#include "Singletons/EventManager.h"

class EventRegistry {
public:
    //player
    static constexpr ge::EventId P1_HEALTH_UPDATE{ge::make_sdbm_hash("P1HealthUpdate")};
    static constexpr ge::EventId P2_HEALTH_UPDATE{ge::make_sdbm_hash("P2HealthUpdate")};
    static constexpr ge::EventId P1_DMG{ge::make_sdbm_hash("P1Dmg")};
    static constexpr ge::EventId P2_DMG{ge::make_sdbm_hash("P2Dmg")};
    //enemies
    static constexpr ge::EventId SCORE_UPDATE{ge::make_sdbm_hash("ScoreUpdate")};
    static constexpr ge::EventId SCORE_UI_UPDATE{ge::make_sdbm_hash("ScoreUIUpdate")};
    //UI
    static constexpr ge::EventId A_BUTTON_PRESSED{ge::make_sdbm_hash("APressed")};
    static constexpr ge::EventId UI_SELECTION_UP{ge::make_sdbm_hash("UISelectionUp")};
    static constexpr ge::EventId UI_SELECTION_DOWN{ge::make_sdbm_hash("UISelectionDown")};
    static constexpr ge::EventId LOBBY_COOP_SELECTED{ge::make_sdbm_hash("CoopSelected")};
    static constexpr ge::EventId LOBBY_SINGLEPLAYER_SELECTED{ge::make_sdbm_hash("SinglePlayerSelected")};
    static constexpr ge::EventId LOBBY_VERSUS_SELECTED{ge::make_sdbm_hash("VersusSelected")};

    static void Initialize() {
        auto &em = ge::EventManager::GetInstance();
        //Player
        em.RegisterEventName(P1_HEALTH_UPDATE, "P1HealthUpdate");
        em.RegisterEventName(P2_HEALTH_UPDATE, "P2HealthUpdate");
        em.RegisterEventName(P1_DMG, "P1Dmg");
        em.RegisterEventName(P2_DMG, "P2Dmg");
        //Enemies
        em.RegisterEventName(SCORE_UPDATE, "ScoreUpdate");
        em.RegisterEventName(SCORE_UI_UPDATE, "ScoreUIUpdate");
        //UI
        em.RegisterEventName(A_BUTTON_PRESSED, "APressed");
        em.RegisterEventName(UI_SELECTION_UP, "UISelectionUp");
        em.RegisterEventName(UI_SELECTION_DOWN, "UISelectionDown");
        em.RegisterEventName(LOBBY_COOP_SELECTED, "CoopSelected");
        em.RegisterEventName(LOBBY_SINGLEPLAYER_SELECTED, "SinglePlayerSelected");
        em.RegisterEventName(LOBBY_VERSUS_SELECTED, "VersusSelected");
    }
};
