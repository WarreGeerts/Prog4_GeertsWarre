#include "EventManager.h"

#include "SDL3/SDL_log.h"

namespace ge {
    bool EventManager::s_IsDestroyed = false;

    EventManager::~EventManager() {
        s_IsDestroyed = true;
    }

    EventHandle EventManager::AttachEvent(const EventId eventId, const std::function<void(const Event &)> &callback) {
        auto &vec = m_EventListeners[static_cast<unsigned int>(eventId)];
        vec.push_back(callback);
        return EventHandle{eventId, vec.size() - 1, true};
    }

    void EventManager::DetachEvent(const EventHandle &handle) {
        if (!handle.valid) return;
        const auto it = m_EventListeners.find(static_cast<unsigned int>(handle.eventId));
        if (it == m_EventListeners.end()) return;

        auto &vec = it->second;
        if (handle.index >= vec.size()) return;

        vec[handle.index] = std::move(vec.back());
        vec.pop_back();
    }

    void EventManager::SendEvent(const Event &event) {
        auto iterator = m_EventListeners.find(static_cast<unsigned int>(event.id));
        if (iterator != m_EventListeners.end()) {
            for (const auto &callback: iterator->second) {
                callback(event);
            }
        }
    }

    void EventManager::RegisterEventName(const EventId id, const std::string &name) {
        m_EventNames[id] = name;
    }

    const std::unordered_map<EventId, std::string> &EventManager::GetRegisteredEvents() const {
        return m_EventNames;
    }

    std::string EventManager::GetEventName(const EventId id) const {
        const auto it = m_EventNames.find(id);
        return (it != m_EventNames.end()) ? it->second : "Unknown Event";
    }
}
