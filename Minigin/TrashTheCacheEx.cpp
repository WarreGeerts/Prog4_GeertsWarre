#include "TrashTheCacheEx.h"
#include <chrono>
#include <iostream>
#include <SDL3/SDL_log.h>
#include <algorithm>
//static vars
int TrashTheCacheEx::size{67108864};
int TrashTheCacheEx::AmountIterationsInt{10};
int TrashTheCacheEx::AmountIterationsGO{100};
//timing vectors
std::vector<long long> TrashTheCacheEx::TimingsInt(11, 0);
std::vector<long long> TrashTheCacheEx::TimingsGO(11, 0);
std::vector<long long> TrashTheCacheEx::TimingsGOAlt(11, 0);

void TrashTheCacheEx::Exercise1() {
    SDL_Log("Exercise1\n");
    RunExperiment<int>(TimingsInt, AmountIterationsInt);
}

void TrashTheCacheEx::Exercise2() {
    SDL_Log("Exercise2.1\n");
    RunExperiment<GameObject3D>(TimingsGO, AmountIterationsGO);
    SDL_Log("Exercise2.2\n");
    RunExperiment<GameObject3DAlt>(TimingsGOAlt, AmountIterationsGO);
}

void TrashTheCacheEx::SetAmountIterationsInt(const int amount) {
    AmountIterationsInt = amount;
}

void TrashTheCacheEx::SetAmountIterationsGO(const int amount) {
    AmountIterationsGO = amount;
}

template<typename T>
void TrashTheCacheEx::RunExperiment(std::vector<long long> &averageTiming, const int iterations) {
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
TrashTheCacheEx::MakeAverage(const std::vector<std::vector<long long> > &timings, const int iterations) {
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

void TrashTheCacheEx::DisplayValues(const std::vector<long long> &timings) {
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
