cmake_minimum_required(VERSION 3.10)
project(sudoku)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-DNDEBUG -O3 -mtune=native -march=native -Wall")

add_executable(solve unittest/solve.cpp board.cpp solver.cpp)
add_executable(generate unittest/generate.cpp board.cpp solver.cpp generator.cpp)
add_executable(bench unittest/benchmark.cpp board.cpp solver.cpp)
add_executable(combined unittest/combined.cpp board.cpp combined/combined_board.cpp combined/combined_solver.cpp)
add_executable(kaggle unittest/kaggle.cpp board.cpp solver.cpp)
