#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
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

        //std::variant ipv union
        union {
            int i;
            float f;
        } args[MAX_ARGS]{};

        explicit Event(const EventId id) : id{id} {}

        Event &AddInt(const int value) {
            args[numArgs++].i = value;
            return *this;
        }
    };

    //Event Id's
    constexpr EventId P1_HEALTH_UPDATE{make_sdbm_hash("P1HealthUpdate")};
    constexpr EventId P2_HEALTH_UPDATE{make_sdbm_hash("P2HealthUpdate")};
    constexpr EventId P1_DMG{make_sdbm_hash("P1Dmg")};
    constexpr EventId P2_DMG{make_sdbm_hash("P2Dmg")};
    constexpr EventId P1_SCORE_UPDATE{make_sdbm_hash("P1ScoreUpdate")};
    constexpr EventId P2_SCORE_UPDATE{make_sdbm_hash("P2ScoreUpdate")};
    constexpr EventId P1_BURGER_FALL{make_sdbm_hash("P1BurgerFall")};
    constexpr EventId P2_BURGER_FALL{make_sdbm_hash("P2BurgerFall")};
    constexpr EventId P1_ENEMY_KILL{make_sdbm_hash("P1EnemyKill")};
    constexpr EventId P2_ENEMY_KILL{make_sdbm_hash("P2EnemyKill")};

    //Event Manager
    struct EventHandle {
        EventId eventId{};
        std::size_t index{};
        bool valid{false};
    };

    class EventManager final : public Singleton<EventManager> {
    public:
        EventHandle AttachEvent(const EventId eventId, const std::function<void(const Event &)> &callback) {
            auto &vec = m_EventListeners[static_cast<unsigned int>(eventId)];
            vec.push_back(callback);
            return EventHandle{eventId, vec.size() - 1, true};
        }

        void DetachEvent(const EventHandle &handle) {
            if (!handle.valid) return;
            const auto it = m_EventListeners.find(static_cast<unsigned int>(handle.eventId));
            if (it == m_EventListeners.end()) return;

            auto &vec = it->second;
            if (handle.index >= vec.size()) return;

            vec[handle.index] = std::move(vec.back());
            vec.pop_back();
        }

        void SendEvent(const Event &event) {
            auto iterator = m_EventListeners.find(static_cast<unsigned int>(event.id));
            if (iterator != m_EventListeners.end()) {
                for (const auto &callback: iterator->second) {
                    callback(event);
                }
            }
        }

    private:
        friend Singleton<EventManager>;
        EventManager() = default;
        std::unordered_map<unsigned int, std::vector<std::function<void(const Event &)> > > m_EventListeners;
    };
}
