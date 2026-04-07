#pragma once
#include <vector>
#include "Component.h"
#include "imgui.h"

namespace dae {
    struct Transform {
        float matrix[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    };

    class GameObject3D {
    public:
        Transform transform;
        int ID;
    };

    class GameObject3DAlt {
    public:
        Transform *transform;
        int ID;
    };

    class ThrashCacheComponent final : public Component {
    public:
        explicit ThrashCacheComponent(GameObject *go) : Component(go,"ThrashCacheComponent") {
            constexpr auto AmountSteps{11};
            TimingsInt.reserve(AmountSteps);
            TimingsGO.reserve(AmountSteps);
            TimingsGOAlt.reserve(AmountSteps);

            for (int i = 0; i < AmountSteps; i++) {
                TimingsInt.push_back(0);
                TimingsGO.push_back(0);
                TimingsGOAlt.push_back(0);
            }
        }
        void Render() const override {};
        void RenderGUI() override{};
        void Update() override {};
        void InspectorGUI() override;
        void SetAmountIterationsInt(int amount);
        void SetAmountIterationsGO(int amount);

    private:
        //main helper function (making DRY code :p)
        template<typename T>
        void RunExperiment(std::vector<long long> &averageTiming, int iterations);
        static void PlotHelper(const ImVec4 &color, const std::vector<long long> &Timings, const ImVec4 &color2 = ImVec4{},
                               const std::vector<long long> &Timings2 = std::vector<long long>{});
        //helper functions
        static void Operate(int &value) { value *= 2; }
        static void Operate(GameObject3D &go) { go.ID *= 2; }
        static void Operate(GameObject3DAlt &go) { go.ID *= 2; }
        static void MakeAverage(const std::vector<std::vector<long long> > &timings, std::vector<long long> &averageTiming);
        //static vars
        int size{67108864}; //2^26
        int AmountIterationsInt{10};
        int AmountIterationsGO{100};
        std::vector<long long> TimingsInt{};
        std::vector<long long> TimingsGO{};
        std::vector<long long> TimingsGOAlt{};
        //Imgui
        bool ShowTimingsInt{false};
        bool ShowTimingsGO{false};
        bool ShowTimingsGOAlt{false};
    };
}
