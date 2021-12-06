#include <vector>
#include <any>
#include <ctime>
#include <algorithm>
#include <fstream>

#include "benchmark/benchmark.h"

#include "src/tournament_sort.h"
#include "src/limited_RAM.h"

using namespace std;

bool is_sorted(const vector<int>& array) {
    if (array.size() <= 1) {
        return true;
    }

    for (int i = 0; i < array.size() - 1; i++) {
        if (array[i] > array[i + 1]) {
            return false;
        }
    }
    return true;
}

auto get_random_int = [](int min, int max) -> int {
    return min + rand() % (( max + 1 ) - min);
};

vector<int> generate_random_array(size_t size) {
    vector<int> array(size);
    for (size_t i = 0; i < size; i++) {
        array[i] = get_random_int(1, size);
    }
    return array;
}

void setup(const benchmark::State&) {
    static bool srand_called = false;

    if (!srand_called) {
        srand(time(nullptr));
        srand_called = true;
    }
}

void BM_tournament_sort(benchmark::State& state) {
    size_t array_size = state.range(0);
    state.SetComplexityN(state.range(0));

    vector<int> array;
    vector<int> sorted;

    for (auto _ : state) {
        state.PauseTiming();
        array = generate_random_array(array_size);
        state.ResumeTiming();

        sorted = tournament_sort(array);
        state.PauseTiming();

        if (!is_sorted(sorted) || array.size() != sorted.size()) {
            state.SkipWithError("Not sorted");
        }
    }
}

void BM_std_sort(benchmark::State& state) {
    size_t array_size = state.range(0);
    state.SetComplexityN(state.range(0));

    vector<int> array;
    vector<int> sorted;

    for (auto _ : state) {
        state.PauseTiming();
        array = generate_random_array(array_size);
        state.ResumeTiming();

        sort(array.begin(), array.end());
        state.PauseTiming();

        if (!is_sorted(sorted) || array.size() != sorted.size()) {
            state.SkipWithError("Not sorted");
        }
    }
}

BENCHMARK(BM_tournament_sort)->Unit(benchmark::kMillisecond)->RangeMultiplier(10)->Range(10, 1000000)->Setup(setup)->Complexity(benchmark::oNLogN);
BENCHMARK(BM_std_sort)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(10, 10000000)->Setup(setup)->Complexity(benchmark::oNLogN);

BENCHMARK_MAIN();
