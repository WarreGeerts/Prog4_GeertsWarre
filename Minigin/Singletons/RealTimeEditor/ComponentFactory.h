#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include "Components/Component.h"
#include "Singletons/Singleton.h"

namespace ge {
    class ComponentFactory final : public Singleton<ComponentFactory> {
    public:
        std::unique_ptr<Component> Create(const std::string &type, GameObject *go);

        template<typename T>
        void Register(const std::string &name) {
            GetFactory()[name] = [](GameObject *go) {
                return std::make_unique<T>(go);
            };
        }

        std::vector<std::string> GetRegisteredTypeNames();

    private:
        friend Singleton<ComponentFactory>;
        static std::unordered_map<std::string, std::function<std::unique_ptr<Component>(GameObject *)> > &GetFactory();
    };
}
