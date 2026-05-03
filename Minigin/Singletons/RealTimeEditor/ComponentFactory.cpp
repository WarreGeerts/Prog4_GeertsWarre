#include "ComponentFactory.h"
#include <iostream>
#include <SDL3/SDL_log.h>


using namespace dae;
std::unique_ptr<Component> ComponentFactory::Create(const std::string &type, GameObject *go) {
    auto& factory = GetFactory();
    auto it = factory.find(type);
    if (it != factory.end()) {
        return it->second(go);
    }
    SDL_Log("Unknown type: %s", type.c_str());
    return nullptr;
}

std::unordered_map<std::string, std::function<std::unique_ptr<Component>(GameObject *)>> & ComponentFactory::
GetFactory() {
    static std::unordered_map<std::string, std::function<std::unique_ptr<Component>(GameObject *)>> factory;
    return factory;
}

