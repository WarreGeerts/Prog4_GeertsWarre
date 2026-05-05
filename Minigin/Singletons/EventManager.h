#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include "Singleton.h"

namespace dae {
    //sdbm_hash maker
    template<int length>
    struct sdbm_hash {
        consteval static unsigned int _calculate(const char *const text, unsigned int &value) {
            const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
            value = character + (value << 6) + (value << 16) - value;
            return text[length - 1];
        }

        consteval static unsigned int calculate(const char *const text) {
            unsigned int value{0};
            const auto character{_calculate(text, value)};
            return character + (value << 6) + (value << 16) - value;
        }
    };

    template<>
    struct sdbm_hash<1> {
        consteval static int _calculate(const char *const text, unsigned int &) { return text[0]; }
    };

    template<size_t N>
    consteval unsigned int make_sdbm_hash(const char (&text)[N]) {
        return sdbm_hash<N - 1>::calculate(text);
    }

    using EventId = unsigned int;

    struct Event {
        EventId id;
        static constexpr uint8_t MAX_ARGS{8};
        uint8_t numArgs{0};
        std::variant<int, float> args[MAX_ARGS];
        explicit Event(const EventId id) : id{id} {}

        Event &AddInt(const int value) {
            args[numArgs++] = value;
            return *this;
        }
    };

    //Event Manager
    struct EventHandle {
        EventId eventId{};
        std::size_t index{};
        bool valid{false};
    };

    class EventManager final : public Singleton<EventManager> {
    public:
        ~EventManager() override;
        static bool isAlive() { return !s_IsDestroyed; }
        EventHandle AttachEvent(EventId eventId, const std::function<void(const Event &)> &callback);
        void DetachEvent(const EventHandle &handle);
        void SendEvent(const Event &event);
        void RegisterEventName(EventId id, const std::string &name);
        const std::unordered_map<EventId, std::string> &GetRegisteredEvents() const;
        std::string GetEventName(EventId id) const;

    private:
        friend Singleton<EventManager>;
        EventManager() = default;
        static bool s_IsDestroyed;
        std::unordered_map<unsigned int, std::vector<std::function<void(const Event &)> > > m_EventListeners;
        std::unordered_map<EventId, std::string> m_EventNames;
    };

    class EventRegistry {
    public:
        //Event Id's
        static constexpr EventId P1_HEALTH_UPDATE{make_sdbm_hash("P1HealthUpdate")};
        static constexpr EventId P2_HEALTH_UPDATE{make_sdbm_hash("P2HealthUpdate")};
        static constexpr EventId P1_DMG{make_sdbm_hash("P1Dmg")};
        static constexpr EventId P2_DMG{make_sdbm_hash("P2Dmg")};
        static constexpr EventId P1_SCORE_UPDATE{make_sdbm_hash("P1ScoreUpdate")};
        static constexpr EventId P2_SCORE_UPDATE{make_sdbm_hash("P2ScoreUpdate")};
        static constexpr EventId P1_BURGER_FALL{make_sdbm_hash("P1BurgerFall")};
        static constexpr EventId P2_BURGER_FALL{make_sdbm_hash("P2BurgerFall")};
        static constexpr EventId P1_ENEMY_KILL{make_sdbm_hash("P1EnemyKill")};
        static constexpr EventId P2_ENEMY_KILL{make_sdbm_hash("P2EnemyKill")};

        static void Initialize() {
            auto &em = EventManager::GetInstance();

            em.RegisterEventName(P1_HEALTH_UPDATE, "P1HealthUpdate");
            em.RegisterEventName(P2_HEALTH_UPDATE, "P2HealthUpdate");
            em.RegisterEventName(P1_DMG, "P1Dmg");
            em.RegisterEventName(P2_DMG, "P2Dmg");
            em.RegisterEventName(P1_SCORE_UPDATE, "P1ScoreUpdate");
            em.RegisterEventName(P2_SCORE_UPDATE, "P2ScoreUpdate");
            em.RegisterEventName(P1_BURGER_FALL, "P1BurgerFall");
            em.RegisterEventName(P2_BURGER_FALL, "P2BurgerFall");
            em.RegisterEventName(P1_ENEMY_KILL, "P1EnemyKill");
            em.RegisterEventName(P2_ENEMY_KILL, "P2EnemyKill");
        }

        static std::vector<std::string> GetAllEventNames();
    };
}
