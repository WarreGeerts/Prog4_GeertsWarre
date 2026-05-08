#include "ComponentFactory.h"
#include <SDL3/SDL_log.h>
namespace ge {
    std::unique_ptr<Component> ComponentFactory::Create(const std::string &type, GameObject *go) {
        auto &factory = GetFactory();
        auto it = factory.find(type);
        if (it != factory.end()) {
            return it->second(go);
        }
        SDL_Log("Unknown type: %s", type.c_str());
        return nullptr;
    }

    std::vector<std::string> ComponentFactory::GetRegisteredTypeNames() {
        std::vector<std::string> names;
        for (const auto &[name, creator]: GetFactory()) {
            names.push_back(name);
        }
        return names;
    }

    std::unordered_map<std::string, std::function<std::unique_ptr<Component>(GameObject *)> > &ComponentFactory::
    GetFactory() {
        static std::unordered_map<std::string, std::function<std::unique_ptr<Component>(GameObject *)> > factory;
        return factory;
    }
}