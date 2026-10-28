cmake_minimum_required(VERSION 3.21)
project(OptimizationLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Статична бібліотека
add_library(OptimizationLib STATIC
    src/FunctionAnalysis.cpp
    src/OptimizationTask.cpp
)

target_include_directories(OptimizationLib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Демонстраційна програма
add_executable(OptimizationDemo demo/main.cpp)
target_link_libraries(OptimizationDemo PRIVATE OptimizationLib)

# Демонстрація антипатернів
add_executable(AntipatternDemo antipattern/main.cpp)

# Google Test
find_package(GTest CONFIG REQUIRED)
enable_testing()

add_executable(OptimizationTests tests/OptimizationTests.cpp)
target_link_libraries(OptimizationTests PRIVATE OptimizationLib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(OptimizationTests)