cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER clang++)

project(test_fibonacci_heap CXX)

add_executable(${PROJECT_NAME} test_fibonacci_heap.cpp fibonacci_heap.h fibonacci_heap.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -std=c++17)

if(ENABLE_UBASAN)
    target_compile_options(${PROJECT_NAME} PUBLIC -g -O0 -fsanitize=address,undefined,leak -fno-omit-frame-pointer)
    target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address,undefined,leak)
endif()

if(ENABLE_MSAN)
    target_compile_options(${PROJECT_NAME} PUBLIC -g -O0 -fsanitize=memory -fPIE -fno-omit-frame-pointer)
    target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=memory)
endif()

if(CHECK_COVERAGE)
    target_compile_options(${PROJECT_NAME} PUBLIC -g -O0 -fprofile-arcs -ftest-coverage)
    target_link_options(${PROJECT_NAME} PUBLIC -lgcov --coverage)
endif()

enable_testing()
add_subdirectory(googletest)
target_link_libraries(${PROJECT_NAME} gtest_main)