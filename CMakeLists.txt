cmake_minimum_required(VERSION 3.20)
project(MyImplementations LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MyImplementations INTERFACE)

target_include_directories(MyImplementations INTERFACE include)

# --- Testing ---
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests/Vector_test.cpp tests/Ring_Buffer_test.cpp)
target_link_libraries(tests PRIVATE MyImplementations GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)