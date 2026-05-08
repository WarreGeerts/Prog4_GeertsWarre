#pragma once
#include <memory>

namespace ge {
    class Controller {
    public:
        explicit Controller(uint32_t controllerIndex);
        //Controller();
        ~Controller();

        [[nodiscard]] bool IsConnected() const;
        void Update();
        void SetControllerIndex(uint32_t idx) const;

        [[nodiscard]] bool IsPressed(int button) const;
        [[nodiscard]] bool IsDownThisFrame(int button) const;
        [[nodiscard]] bool IsUpThisFrame(int button) const;

    private:
        class ControllerImpl;
        std::unique_ptr<ControllerImpl> m_pImpl;
    };
}
