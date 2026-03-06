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
        explicit ThrashCacheComponent(GameObject *go) : Component(go) {}
        void Render() const override;
        void Update() override;
        void SetAmountIterationsInt(int amount);
        void SetAmountIterationsGO(int amount);
        void PlotExercise1();
        void PlotExercise2();

    private:
        //main helper function (making DRY code :p)
        template<typename T>
        void RunExperiment(std::vector<long long> &averageTiming, int iterations);
        void PlotHelper(const ImVec4 &color, const std::vector<long long> &Timings, const ImVec4 &color2 = ImVec4{},
                        const std::vector<long long> &Timings2 = std::vector<long long>{});
        //helper functions
        void Operate(int &value) { value *= 2; }
        void Operate(GameObject3D &go) { go.ID *= 2; }
        void Operate(GameObject3DAlt &go) { go.ID *= 2; }
        std::vector<long long> MakeAverage(const std::vector<std::vector<long long> > &timings, int iterations);
        void DisplayValues(const std::vector<long long> &timings);
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
