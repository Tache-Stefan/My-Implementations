cmake_minimum_required(VERSION 3.20)
project(MyImplementations LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MyImplementations INTERFACE)

target_include_directories(MyImplementations INTERFACE include)

include(FetchContent)

# --- Testing ---
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

# --- Benchmarks ---
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.5
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest benchmark)

add_executable(benchmarks
    benchmarks/LF_Ring_Buffer_bench.cpp
)
target_link_libraries(benchmarks PRIVATE MyImplementations benchmark::benchmark_main)

enable_testing()

add_executable(tests
    tests/Vector_test.cpp 
    tests/Ring_Buffer_test.cpp 
    tests/Array_test.cpp
    tests/List_test.cpp
    tests/LF_Ring_Buffer_test.cpp
    )
target_link_libraries(tests PRIVATE MyImplementations GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)