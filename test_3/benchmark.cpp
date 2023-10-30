#include "../functions.h"

#include <benchmark/benchmark.h>

#include <array>
#include <cstdint>
#include <iterator>
#include <random>

auto test_cases = [](){
    std::minstd_rand generator;
    std::array<std::array<int32_t, 3>, 32768> ns;
    std::uniform_int_distribution<> sdist(-10000, 10000);
    for (auto& n : ns) {
        n[0] = sdist(generator);
        n[1] = sdist(generator);
        n[2] = sdist(generator);
    }
    return ns;
}();

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
    ::benchmark::Initialize(&argc, argv);

    RegisterSortBenchmarks(functions);

    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}