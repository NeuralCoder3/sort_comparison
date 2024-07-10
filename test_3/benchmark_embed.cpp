#include "../functions.h"

#include <benchmark/benchmark.h>

#include <array>
#include <cstdint>
#include <iterator>
#include <random>

// int test_count = 401;
// int min_length = 3;
// int max_length = 20000;

int test_count = 4001;
int min_length = 3;
int max_length = 200;

#define QUICKSORT
// #define MERGESORT

std::vector<std::vector<int32_t>> test_cases;


// // Mergesort but use func for length 3, handle <= 2 manually
// static void MergeSort3(int32_t* data, int32_t* buffer, int32_t length, const TestFunction& func) {
//     if (length <= 2) {
//         if (length == 2 && data[0] > data[1]) {
//             std::swap(data[0], data[1]);
//         }
//         return;
//     }
//     if (length == 3) {
//         func.func(data);
//         return;
//     }
//     int32_t mid = length / 2;
//     MergeSort3(data, buffer, mid, func);
//     MergeSort3(data + mid, buffer, length - mid, func);
//     std::merge(data, data + mid, data + mid, data + length, buffer);
//     std::copy(buffer, buffer + length, data);
// }

int partition(int arr[], int start, int end) {
    int pivot = arr[start];
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }
    int pivotIndex = start + count;
    std::swap(arr[pivotIndex], arr[start]);
    int i = start, j = end;
    while (i < pivotIndex && j > pivotIndex) {
        while (arr[i] <= pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i < pivotIndex && j > pivotIndex) {
            std::swap(arr[i++], arr[j--]);
        }
    }
    return pivotIndex;
}
 
void quickSort(int* arr, int start, int end, const TestFunction& func)
{
    if (start >= end)
        return;
    if (end - start + 1 == 3) {
        func.func(arr + start);
        return;
    }
    int p = partition(arr, start, end);
    quickSort(arr, start, p - 1, func);
    quickSort(arr, p + 1, end, func);
}



// maybe use std::merge or std::inplace_merge
void merge(int array[], int const left, int const mid,
           int const right)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;
 
    // Create temp arrays
    auto *leftArray = new int[subArrayOne],
         *rightArray = new int[subArrayTwo];
 
    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];
 
    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;
 
    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne
           && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne]
            <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray]
                = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray]
                = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
 
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray]
            = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
 
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray]
            = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}
 
// begin is for left index and end is right index
// of the sub-array of arr to be sorted
void mergeSort(int array[], int const begin, int const end, const TestFunction& func)
{
    if (begin >= end)
        return;
    
    if (end - begin + 1 == 3) {
        func.func(array + begin);
        return;
    }
    if (end - begin + 1 == 2) {
        if (array[begin] > array[end]) {
            std::swap(array[begin], array[end]);
        }
        return;
    }
 
    int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid, func);
    mergeSort(array, mid + 1, end, func);
    merge(array, begin, mid, end);
}






static void BenchmarkSortFunction(benchmark::State& state, const TestFunction& testFunc) {
    for (auto _ : state) {
        for (auto p : test_cases) {
            // testFunc.func(p.data());
            #ifdef QUICKSORT
            quickSort(p.data(), 0, p.size() - 1, testFunc);
            #endif
            #ifdef MERGESORT
            mergeSort(p.data(), 0, p.size() - 1, testFunc);
            #endif
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

    // for each permutation of 1..5
    // for (int i = 0; i < 120; i++) {
    //     int* arr = new int[5]{1, 2, 3, 4, 5};
    //     int j = i;
    //     for (int k = 1; k <= 5; k++) {
    //         int l = j % k;
    //         j /= k;
    //         std::swap(arr[k - 1], arr[l]);
    //     }
    //     for (int k = 0; k < 5; k++) {
    //         printf("%d ", arr[k]);
    //     }
    //     printf(" -> ");
    //     quickSort(arr, 0, 4, functions[0]);
    //     for (int k = 0; k < 5; k++) {
    //         printf("%d ", arr[k]);
    //     }
    //     printf("\n");
    // }

    // optionally first param is max_length, second is test_count
    if (argc > 1) {
        max_length = atoi(argv[1]);
    }
    if (argc > 2) {
        test_count = atoi(argv[2]);
    }

    std::cout << "min_length = " << min_length << "\n";
    std::cout << "max_length = " << max_length << "\n";
    std::cout << "test_count = " << test_count << "\n";
    #ifdef QUICKSORT
    std::cout << "sort = quicksort\n";
    #endif
    #ifdef MERGESORT
    std::cout << "sort = mergesort\n";
    #endif


    {
    std::minstd_rand generator;

    test_cases = std::vector<std::vector<int32_t>>(test_count);
    std::uniform_int_distribution<> ldist(min_length, max_length);
    std::uniform_int_distribution<> sdist(-10000, 10000);
    for (auto& n : test_cases) {
        int length = ldist(generator);
        for (int i = 0; i < length; i++) {
            n.push_back(sdist(generator));
        }
    }
    };

    ::benchmark::Initialize(&argc, argv);

    RegisterSortBenchmarks(functions);

    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}