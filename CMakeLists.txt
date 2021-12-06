cmake_minimum_required(VERSION 3.20)
project(algorithms_complexity)

set(CMAKE_CXX_STANDARD 20)

execute_process(COMMAND setupExternalDeps.sh)

set(benchmark_DIR "external/benchmark/build")
find_package(benchmark)

add_executable(algorithms_complexity main.cpp src/Logger.h src/Logger.cpp src/tournament_sort.cpp src/tournament_sort.h src/limited_RAM.cpp src/limited_RAM.h)

target_link_libraries(algorithms_complexity benchmark::benchmark)
