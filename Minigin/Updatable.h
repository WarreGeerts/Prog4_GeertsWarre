#pragma once

namespace ge {
    class Updatable {
    public:
        virtual ~Updatable() = default;
        virtual void Update() = 0;
    };
}