#pragma once
namespace ge {
    class Command {
    public:
        virtual ~Command() = default;
        virtual void Execute() = 0;
    };
}
