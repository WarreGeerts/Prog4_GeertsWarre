#include "GSLobby.h"

#include "GMSCoop.h"
#include "GMSManager.h"
#include "GMSSinglePlayer.h"
#include "GMSVersus.h"
#include "GSGame.h"
#include "GSManager.h"
#include "../Events.h"
#include "Singletons/SceneManager.h"

namespace game {
    void GSLobby::OnEnter() {
        ge::SceneManager::GetInstance().SelectSceneByName("Lobby");
        HandleInput();
    }

    void GSLobby::OnExit() {
        //detach when leaving the scene
        ge::EventManager::GetInstance().DetachEvent(m_HandleSP);
        ge::EventManager::GetInstance().DetachEvent(m_HandleCO);
        ge::EventManager::GetInstance().DetachEvent(m_HandleVS);
    }

    void GSLobby::HandleInput() {
        m_HandleSP = ge::EventManager::GetInstance().AttachEvent(
           EventRegistry::LOBBY_SINGLEPLAYER_SELECTED,
           [](const ge::Event &) {
              GSManager::GetInstance().ChangeState(std::make_unique<GSGame>());
              GMSManager::GetInstance().ChangeState(std::make_unique<GMSSinglePlayer>());
           });

        m_HandleCO = ge::EventManager::GetInstance().AttachEvent(
           EventRegistry::LOBBY_COOP_SELECTED,
           [](const ge::Event &) {
              GSManager::GetInstance().ChangeState(std::make_unique<GSGame>());
              GMSManager::GetInstance().ChangeState(std::make_unique<GMSCoop>());
           });

        m_HandleVS = ge::EventManager::GetInstance().AttachEvent(
           EventRegistry::LOBBY_VERSUS_SELECTED,
           [](const ge::Event &) {
              GSManager::GetInstance().ChangeState(std::make_unique<GSGame>());
              GMSManager::GetInstance().ChangeState(std::make_unique<GMSVersus>());
           });
    }
}
