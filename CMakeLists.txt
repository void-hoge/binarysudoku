cmake_minimum_required(VERSION 3.10)
project(sudoku)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-DNDEBUG -O3 -mtune=native -march=native -Wall")

add_executable(sudoku main.cpp board.cpp solver.cpp)
add_executable(bench benchmark.cpp board.cpp solver.cpp)
add_executable(generate generate.cpp board.cpp solver.cpp generator.cpp)
