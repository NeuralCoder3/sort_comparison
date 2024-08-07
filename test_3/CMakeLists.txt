# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2023 Cassio Neri <cassio.neri@gmail.com>
#
# This code is a supplementary material to:
#
#     Neri, C. "Shorter and faster than Sort3AlphaDev", to appear.
#     Preprint: https://arxiv.org/abs/2307.14503.

cmake_minimum_required(VERSION 3.1)

project(sort3
  VERSION 1.0
  DESCRIPTION "Shorter and faster than Sort3AlphaDev"
  LANGUAGES CXX
)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "s;S")

add_compile_options(-fno-pie)
add_link_options(-no-pie)

#---------------------------------------------------------------------------
# Google benchmark
#---------------------------------------------------------------------------

include(FetchContent)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.2
  SOURCE_DIR     ${CMAKE_BINARY_DIR}/tp/benchmark
)

set(BENCHMARK_ENABLE_TESTING off)
FetchContent_MakeAvailable(benchmark)
target_compile_definitions(benchmark PUBLIC -DBENCHMARK_STATIC_DEFINE)

#---------------------------------------------------------------------------
# Sort3
#---------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(bench
  benchmark.cpp
)

add_executable(bench4
  benchmark4.cpp
)

add_executable(bench5
  benchmark5.cpp
)

add_executable(bench_embed
  benchmark_embed.cpp
)

add_executable(bench_embed4
  benchmark_embed4.cpp
)

target_link_libraries(bench benchmark benchmark_main)
target_link_libraries(bench_embed benchmark benchmark_main)
target_link_libraries(bench4 benchmark benchmark_main)
target_link_libraries(bench_embed4 benchmark benchmark_main)
target_link_libraries(bench5 benchmark benchmark_main)

# add libsort_rust.a (${CMAKE_SOURCE_DIR}/../rust_sort/target/release/libsort_rust.a)
# additionally compile .a file into the executable

target_link_libraries(bench ${CMAKE_SOURCE_DIR}/../rust_sort/target/release/libsort_rust.a)
target_link_libraries(bench_embed ${CMAKE_SOURCE_DIR}/../rust_sort/target/release/libsort_rust.a)
target_link_libraries(bench4 ${CMAKE_SOURCE_DIR}/../rust_sort/target/release/libsort_rust.a)
target_link_libraries(bench_embed4 ${CMAKE_SOURCE_DIR}/../rust_sort/target/release/libsort_rust.a)
target_link_libraries(bench5 ${CMAKE_SOURCE_DIR}/../rust_sort/target/release/libsort_rust.a)
