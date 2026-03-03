#pragma once
#include <string>
#include <vector>

#include "imgui.h"
struct ImVec4;

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

class TrashTheCacheEx {
public:
    static void Exercise1();
    static void Exercise2();
    static void Exercise2p2();
    static void SetAmountIterationsInt(int amount);
    static void SetAmountIterationsGO(int amount);

    static void PlotExercise1();
    static void PlotExercise2();

private:
    //main helper function (making DRY code :p)
    template<typename T>
    static void RunExperiment(std::vector<long long> &averageTiming, int iterations);
    static void PlotHelper(const ImVec4& color, const std::vector<long long> &Timings, const ImVec4& color2 = ImVec4{}, const std::vector<long long> &Timings2 = std::vector<long long>{});
    //helper functions
    static void Operate(int &value) { value *= 2; }
    static void Operate(GameObject3D &go) { go.ID *= 2; }
    static void Operate(GameObject3DAlt &go) { go.ID *= 2; }
    static std::vector<long long> MakeAverage(const std::vector<std::vector<long long> > &timings, int iterations);
    static void DisplayValues(const std::vector<long long> &timings);
    //static vars
    static int size; //2^26
    static int AmountIterationsInt;
    static int AmountIterationsGO;
    static std::vector<long long> TimingsInt;
    static std::vector<long long> TimingsGO;
    static std::vector<long long> TimingsGOAlt;
    //Imgui
    static bool ShowTimingsInt;
    static bool ShowTimingsGO;
    static bool ShowTimingsGOAlt;
};
