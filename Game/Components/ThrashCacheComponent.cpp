#include "ThrashCacheComponent.h"
#include <chrono>
#include <SDL3/SDL_log.h>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "implot.h"
using namespace dae;
nlohmann::ordered_json ThrashCacheComponent::Serialize() const {
    nlohmann::ordered_json data;
    data["iterations_int"] = m_AmountIterationsInt;
    data["iterations_go"] = m_AmountIterationsGO;
    data["size"] = m_Size;
    return data;
}

void ThrashCacheComponent::Deserialize(const nlohmann::ordered_json &data) {
    m_AmountIterationsInt = data.value("iterations_int", 10);
    m_AmountIterationsGO = data.value("iterations_go", 100);
    m_Size = data.value("size", 67108864);

}

void ThrashCacheComponent::InspectorGUI() {
    ImGui::Text("Plot Int Timings");
    ImGui::InputInt("# samples##int", &m_AmountIterationsInt, 1, 10);
    if (ImGui::Button("Thrash the cache")) {
        RunExperiment<int>(TimingsInt, m_AmountIterationsInt);
        ShowTimingsInt = true;
    }
    if (ShowTimingsInt)
        PlotHelper(ImVec4(1.0f, 0.647f, 0.0f, 1.0f), TimingsInt);

    ImGui::Text(" ");
    ImGui::Text("Plot GameObject Timings");
    ImGui::InputInt("# samples##Go", &m_AmountIterationsGO, 1, 100);
    if (ImGui::Button("Thrash the cache with GameObject3D")) {
        RunExperiment<GameObject3D>(TimingsGO, m_AmountIterationsGO);
        ShowTimingsGO = true;
    }
    if (ImGui::Button("Thrash the cache with GameObject3DAlt")) {
        RunExperiment<GameObject3DAlt>(TimingsGOAlt, m_AmountIterationsGO);
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
}

void ThrashCacheComponent::SetAmountIterationsInt(const int amount) {
    m_AmountIterationsInt = amount;
}

void ThrashCacheComponent::SetAmountIterationsGO(const int amount) {
    m_AmountIterationsGO = amount;
}

void ThrashCacheComponent::PlotHelper(const ImVec4 &color, const std::vector<long long> &Timings,
                                      const ImVec4 &color2, const std::vector<long long> &Timings2) {
    if (ImPlot::BeginPlot("##TimingPlot", ImVec2(350, 150), ImPlotFlags_NoInputs)) {
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
    std::vector<T> array(m_Size, T{});
    std::vector<std::vector<long long> > timings;

    int step{0};

    for (step = 1; step <= 1024; step *= 2) {
        std::vector<long long> exp;
        for (int iteration{0}; iteration < iterations; ++iteration) {
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

    MakeAverage(timings, averageTiming);
}

void ThrashCacheComponent::MakeAverage(const std::vector<std::vector<long long> > &timings,
                                       std::vector<long long> &averageTiming) {
    auto iteration{0};
    for (auto &timing: timings) {
        auto [fst, snd] = minmax_element(timing.begin(), timing.end());
        const auto sum{std::accumulate(timing.begin(), timing.end(), 0LL) - (*fst) - (*snd)};
        averageTiming[iteration] = (sum / static_cast<long long>(timings.size() - 2));
        ++iteration;
    }
}
