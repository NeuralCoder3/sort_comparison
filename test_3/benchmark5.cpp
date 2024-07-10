#include "../functions5.h"

#include <benchmark/benchmark.h>

#include <array>
#include <cstdint>
#include <iterator>
#include <random>

int test_count = 4001;
// int test_count = 401;

// cmake --preset=clang.release.make
// cmake --build build/clang.release.make
// ./build/clang.release.make/bin/bench4 | tee -a times/4_all_bench_clang_release_make_bench4_4001
// ./build/clang.release.make/bin/bench_embed4 | tee -a times/4_quick_all_bench_clang_release_make_bench_embed4_4001

std::vector<std::array<int32_t, 5>> test_cases;

static void BenchmarkSortFunction(benchmark::State &state, const TestFunction &testFunc)
{
    for (auto _ : state)
    {
        for (auto p : test_cases)
        {
            testFunc.func(p.data());
            benchmark::DoNotOptimize(p.data());
        }
    }
}

void RegisterSortBenchmarks(std::vector<TestFunction> &functions)
{
    for (const auto &func : functions)
    {
        benchmark::RegisterBenchmark(
            func.name.c_str(),
            [testFunc = func](benchmark::State &state)
            {
                BenchmarkSortFunction(state, testFunc);
            });
    }
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        test_count = atoi(argv[1]);
    }

    std::cout << "test_count = " << test_count << "\n";

    {
        std::minstd_rand generator;
        test_cases = std::vector<std::array<int32_t, 5>>(test_count);
        std::uniform_int_distribution<> sdist(-10000, 10000);
        for (auto &n : test_cases)
        {
            n[0] = sdist(generator);
            n[1] = sdist(generator);
            n[2] = sdist(generator);
            n[3] = sdist(generator);
            n[4] = sdist(generator);
        }
    };

    ::benchmark::Initialize(&argc, argv);

    RegisterSortBenchmarks(functions);

    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}
