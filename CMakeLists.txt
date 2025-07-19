project(pnl_calculator)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2")

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(pnl_calculator 
    src/main.cpp 
    src/PnLCalculator.cpp
)
target_include_directories(pnl_calculator PRIVATE include)

add_executable(pnl_calculator_tests
    test/test_PnLCalculator.cpp
    src/PnLCalculator.cpp
)
target_include_directories(pnl_calculator_tests PRIVATE include)
target_link_libraries(pnl_calculator_tests PRIVATE gtest_main)

add_test(NAME pnl_calculator_tests COMMAND pnl_calculator_tests)