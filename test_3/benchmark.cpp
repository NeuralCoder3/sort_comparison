#include "../functions.h"

#include <benchmark/benchmark.h>

#include <array>
#include <cstdint>
#include <iterator>
#include <random>

int test_count = 401;

std::vector<std::array<int32_t, 3>> test_cases; 

static void BenchmarkSortFunction(benchmark::State& state, const TestFunction& testFunc) {
    for (auto _ : state) {
        for (auto p : test_cases) {
            testFunc.func(p.data());
            benchmark::DoNotOptimize(p.data());
        }
    }
}

void RegisterSortBenchmarks(std::vector<TestFunction>& functions) {
    for (const auto& func : functions) {
        benchmark::RegisterBenchmark(
            func.name.c_str(),
            [testFunc = func](benchmark::State& state) {
                BenchmarkSortFunction(state, testFunc);
            });
    }
}

int main(int argc, char** argv) {
    if (argc > 1) {
        test_count = atoi(argv[1]);
    }

    std::cout << "test_count = " << test_count << "\n";

    {
        std::minstd_rand generator;
        test_cases = std::vector<std::array<int32_t, 3>>(test_count);
        std::uniform_int_distribution<> sdist(-10000, 10000);
        for (auto& n : test_cases) {
            n[0] = sdist(generator);
            n[1] = sdist(generator);
            n[2] = sdist(generator);
        }
    };

    ::benchmark::Initialize(&argc, argv);

    RegisterSortBenchmarks(functions);

    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}