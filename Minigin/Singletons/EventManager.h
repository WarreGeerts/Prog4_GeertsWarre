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

    template<size_t N> consteval unsigned int make_sdbm_hash(const char (&text)[N]) {
        return sdbm_hash<N - 1>::calculate(text);
    }

    using EventId = unsigned int;

    struct Event {
        EventId id;
        static constexpr uint8_t MAX_ARGS {8};
        uint8_t numArgs{0};

        union {
            int i;
            float f;
        } args[MAX_ARGS]{};

        explicit Event(const EventId id) : id{id} {}

        Event& AddInt(const int value) {
            args[numArgs++].i = value;
            return *this;
        }
    };

    //Id's
    constexpr EventId PLAYER1_DIED {make_sdbm_hash("Player1Died")};
    constexpr EventId PLAYER2_DIED {make_sdbm_hash("Player2Died")};

    //Event Manager
    class EventManager final : public Singleton<EventManager> {
    public:
        void AttachEvent(const EventId eventId, const std::function<void(const Event&)>& callback) {
            m_EventListeners[static_cast<unsigned int>(eventId)].push_back(callback);
        }

        void SendEvent(const Event& event) {
            auto iterator { m_EventListeners.find(static_cast<unsigned int>(event.id))};
            if (iterator != m_EventListeners.end()) {
                for (const auto& callback: iterator->second) {
                    callback(event);
                }
            }
        }

    private:
        friend Singleton<EventManager>;
        EventManager() = default;
        std::unordered_map<unsigned int, std::vector<std::function<void(const Event&)>>> m_EventListeners;
    };
}

