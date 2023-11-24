# BinarySudoku
- a bitboard sudoku solver

## Build
```
$ clone https://github.com/void-hoge/binarysudoku.git
$ cd binarysudoku
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Depends
- cmake
- g++

## Problem Format
- Some Sudoku databases express problems using integers from 1 to 9 and `.` as blank.
  - However, this format is not scalable because it cannot be used if the number not have a single digit.
- In this project, `-` is used as blank and each blank or integers are separated by spaces.
- Integers of the output of `board::show()` is 0 to 8 instead of 1 to 9.
  - 0 to f when 16x16.
```
+---------+---------+---------+
|       8 |       4 |         |
|    3    |         |         |
|         |       5 | 6     0 |
+---------+---------+---------+
|       2 |    7    |         |
| 0       |       6 |       1 |
|         |         |    8    |
+---------+---------+---------+
|         | 1  8    |         |
| 5       |         |       7 |
|         |    3    |         |
+---------+---------+---------+
```
- This problem is represented as follow.
```
- - 9 - - 5 - - - - 4 - - - - - - - - - - - - 6 7 - 1 - - 3 - 8 - - - - 1 - - - - 7 - - 2 - - - - - - - 9 - - - - 2 9 - - - - 6 - - - - - - - 8 - - - - 4 - - - -
```

## Solver
- `solve (3|4)`
  - Input a problem from standard input.

```
$ ./solve 3 < ../problems/9x9/q1.txt
+---------+---------+---------+
|       8 |       4 |         |
|    3    |         |         |
|         |       5 | 6     0 |
+---------+---------+---------+
|       2 |    7    |         |
| 0       |       6 |       1 |
|         |         |    8    |
+---------+---------+---------+
|         | 1  8    |         |
| 5       |         |       7 |
|         |    3    |         |
+---------+---------+---------+
+---------+---------+---------+
| 2  0  8 | 3  6  4 | 1  7  5 |
| 6  3  5 | 7  0  1 | 4  2  8 |
| 1  7  4 | 8  2  5 | 6  3  0 |
+---------+---------+---------+
| 3  1  2 | 4  7  8 | 0  5  6 |
| 0  8  7 | 2  5  6 | 3  4  1 |
| 4  5  6 | 0  1  3 | 7  8  2 |
+---------+---------+---------+
| 7  4  0 | 1  8  2 | 5  6  3 |
| 5  2  3 | 6  4  0 | 8  1  7 |
| 8  6  1 | 5  3  7 | 2  0  4 |
+---------+---------+---------+
guesses: 0
updates: 5
elapsed: 38 microseconds
$
```

## Generator
- `generate (3|4) <clues> <seed>`
  - If seed is not entered, it is selected randomly.
  - NOTE: Minimum clues of Sudoku
	- 9x9: 17 (proven)
	- 16x16: 55
	
```
seed: 1522182460
clues: 17
attempting................................................................................................................................................................................................................................................................................................................................................................................................
+---------+---------+---------+
|    6  5 |         | 7       |
|    1    |         |         |
|         |    2    |    3    |
+---------+---------+---------+
|         |    4  3 |         |
|    5  6 |         | 1       |
|         |    0    |         |
+---------+---------+---------+
| 4       |         |    2  3 |
|         |         |    4    |
|         | 6       |         |
+---------+---------+---------+
+---------+---------+---------+
| 8  6  5 | 4  3  0 | 7  1  2 |
| 2  1  3 | 7  6  5 | 4  8  0 |
| 0  4  7 | 8  2  1 | 5  3  6 |
+---------+---------+---------+
| 1  0  8 | 5  4  3 | 2  6  7 |
| 3  5  6 | 2  7  8 | 1  0  4 |
| 7  2  4 | 1  0  6 | 3  5  8 |
+---------+---------+---------+
| 4  8  1 | 0  5  7 | 6  2  3 |
| 6  7  0 | 3  1  2 | 8  4  5 |
| 5  3  2 | 6  8  4 | 0  7  1 |
+---------+---------+---------+
384 attempts in 12.5509 seconds (30.5954 attempts per second)
solutions: 1
guesses: 0
$
```

## Benchmark
- `bench <filename>`
- For performance benchmarking
- It's support only 9x9.

```
$ ./bench ../problems/17clues.txt
No.1
elapsed: 53.638 microseconds

No.2
elapsed: 36.247 microseconds

No.3
elapsed: 32.895 microseconds

No.4
.
.
.
.
.
No.49157
elapsed: 39.39 microseconds

No.49158
elapsed: 30.032 microseconds

average time: 34.8236 microseconds
no guess: 41596 / 49158 (84.6169 %)
average guesses: 0.587799
$
```

## Author
- Mugi Noda (void-hoge)

## License
- GPLv3
