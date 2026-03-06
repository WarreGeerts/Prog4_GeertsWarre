#include "ThrashCacheComponent.h"
#include <chrono>
#include <SDL3/SDL_log.h>
#include <algorithm>
#include <cmath>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "implot.h"
using namespace dae;

    void ThrashCacheComponent::Render() const {
        ImGui::Render();
    }

void ThrashCacheComponent::Update() {
        PlotExercise1();
        PlotExercise2();
    }

void ThrashCacheComponent::SetAmountIterationsInt(const int amount) {
    AmountIterationsInt = amount;
}

void ThrashCacheComponent::SetAmountIterationsGO(const int amount) {
    AmountIterationsGO = amount;
}

void ThrashCacheComponent::PlotExercise1() {
    if (ImGui::Begin("Exercise 1")) {
        ImGui::InputInt("# samples", &AmountIterationsInt, 1, 10);
        if (ImGui::Button("Thrash the cache")) {
            RunExperiment<int>(TimingsInt, AmountIterationsInt);
            ShowTimingsInt = true;
        }
        if (ShowTimingsInt)
            PlotHelper(ImVec4(1.0f, 0.647f, 0.0f, 1.0f), TimingsInt);
        ImGui::End();
    }
}

void ThrashCacheComponent::PlotExercise2() {
    if (ImGui::Begin("Exercise 2")) {
        ImGui::InputInt("# samples", &AmountIterationsGO, 1, 100);
        if (ImGui::Button("Thrash the cache with GameObject3D")) {
            RunExperiment<GameObject3D>(TimingsGO, AmountIterationsGO);
            ShowTimingsGO = true;
        }
        if (ImGui::Button("Thrash the cache with GameObject3DAlt")) {
            RunExperiment<GameObject3DAlt>(TimingsGOAlt, AmountIterationsGO);
            ShowTimingsGOAlt = true;
        }
        if (ShowTimingsGO)
            PlotHelper(ImVec4(1.0f, 0.647f, 0.0f, 1.0f), TimingsGO);
        if (ShowTimingsGOAlt)
            PlotHelper(ImVec4(0.0f, 0.647f, 1.0f, 1.0f), TimingsGOAlt);

        if (ShowTimingsGO && ShowTimingsGOAlt)
            PlotHelper(
                ImVec4(1.0f, 0.647f, 0.0f, 1.0f), TimingsGO,
                ImVec4(0.0f, 0.647f, 1.0f, 1.0f), TimingsGOAlt);

        ImGui::End();
    }
}

void ThrashCacheComponent::PlotHelper(const ImVec4 &color, const std::vector<long long> &Timings,
                                      const ImVec4 &color2, const std::vector<long long> &Timings2) {
    if (ImPlot::BeginPlot("", ImVec2(350, 150), ImPlotFlags_NoInputs)) {
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, 10);

        constexpr double x_pos[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const char *x_labels[]{"1", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024"};
        ImPlot::SetupAxisTicks(ImAxis_X1, x_pos, 11, x_labels);

        long long v_maxY{};
        for (const auto &timing: Timings) {
            if (timing > v_maxY) { v_maxY = timing; }
        }
        v_maxY += v_maxY / 3;

        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, static_cast<double>(v_maxY), ImPlotCond_Always);

        for (auto idx{1}; idx < static_cast<int>(Timings.size()); ++idx) {
            const int x1[]{idx - 1, idx};
            const int y1[]{static_cast<int>(Timings[idx - 1]), static_cast<int>(Timings[idx])};
            ImPlot::SetNextLineStyle(color);
            ImPlot::PlotLine("", x1, y1, 2);
        }
        if (!Timings2.empty()) {
            for (auto idx{1}; idx < static_cast<int>(Timings2.size()); ++idx) {
                const int x1[]{idx - 1, idx};
                const int y1[]{static_cast<int>(Timings2[idx - 1]), static_cast<int>(Timings2[idx])};
                ImPlot::SetNextLineStyle(color2);
                ImPlot::PlotLine("", x1, y1, 2);
            }
        }

        ImPlot::EndPlot();
    }
}

template<typename T>
void ThrashCacheComponent::RunExperiment(std::vector<long long> &averageTiming, const int iterations) {
    std::vector<T> array(size, T{});
    std::vector<std::vector<long long> > timings;

    int step{0};

    for (int iteration{0}; iteration < iterations; ++iteration) {
        std::vector<long long> exp;
        for (step = 1; step <= 1024; step *= 2) {
            auto start = std::chrono::high_resolution_clock::now();

            for (auto idx2{0}; idx2 < static_cast<int>(array.size()); idx2 += step) {
                Operate(array[idx2]);
            }

            auto end = std::chrono::high_resolution_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            exp.push_back(elapsed);
        }
        timings.push_back(exp);
    }

    averageTiming = MakeAverage(timings, iterations);
    DisplayValues(averageTiming);
}

std::vector<long long>
ThrashCacheComponent::MakeAverage(const std::vector<std::vector<long long> > &timings, const int iterations) {
    std::vector<long long> averageTimings(11, 0);

    for (auto idx{0}; idx < static_cast<int>(averageTimings.size()); ++idx) {
        std::vector<long long> orderedTimings{};
        for (auto &timing: timings) {
            orderedTimings.push_back(timing[idx]);
        }
        //order small to big
        std::sort(orderedTimings.begin(), orderedTimings.end());
        //remove start and end
        orderedTimings.erase(orderedTimings.begin());
        orderedTimings.pop_back();

        long long sum{0};
        for (const auto &orderedTiming: orderedTimings) {
            sum += orderedTiming;
        }

        averageTimings[idx] = sum / static_cast<long long>(iterations);
    }
    return averageTimings;
}

void ThrashCacheComponent::DisplayValues(const std::vector<long long> &timings) {
    int step{1};
    for (const auto &timing: timings) {
        std::string msg{};
        msg += std::to_string(step);
        msg += "; ";
        msg += std::to_string(timing);

        step *= 2;

        //had to use SDL_Log to log the values in console because std::cout didn't work.
        SDL_Log("%s", msg.c_str());
    }
}
