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
$ ./generate 3 17
seed: 4075076915
clues: 17
attempting................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................
+---------+---------+---------+
|         |    4    |         |
|       5 |         | 0     8 |
| 3       | 1  6    |         |
+---------+---------+---------+
|    4    |         |         |
| 6       | 0       |         |
|         |       5 |       2 |
+---------+---------+---------+
|    8  2 |       7 |         |
|         |         | 6  1    |
|         |         |         |
+---------+---------+---------+
+---------+---------+---------+
| 8  2  7 | 5  4  0 | 1  3  6 |
| 1  6  5 | 2  7  3 | 0  4  8 |
| 3  0  4 | 1  6  8 | 7  2  5 |
+---------+---------+---------+
| 2  4  3 | 7  8  6 | 5  0  1 |
| 6  5  8 | 0  2  1 | 4  7  3 |
| 0  7  1 | 4  3  5 | 8  6  2 |
+---------+---------+---------+
| 4  8  2 | 6  1  7 | 3  5  0 |
| 7  3  0 | 8  5  2 | 6  1  4 |
| 5  1  6 | 3  0  4 | 2  8  7 |
+---------+---------+---------+
- - - - 5 - - - - - - 6 - - - 1 - 9 4 - - 2 7 - - - - - 5 - - - - - - - 7 - - 1 - - - - - - - - - - 6 - - 3 - 9 3 - - 8 - - - - - - - - - 7 2 - - - - - - - - - - 
480 attempts in 16.1006 seconds (29.8126 attempts per second)
solutions: 1
guesses: 0
$
```

## Benchmark
- `bench <filename>`
- For performance benchmarking
- It supports only 9x9.

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

## Combined Sudoku
- Here it is possible to consider problems where multiple Sudokus share sub-grids.
- `combined` is the program that solves this.

```
$ ./combined < ../problems/9x9/combined/double1.txt 
+---------+---------+---------+                    
| 0       | 3     2 |       5 |                    
|         |         |         |                    
|         | 5  8  6 |         |                    
+---------+---------+---------+                    
| 7     5 |         | 4     1 |                    
|       6 |    7    | 3       |                    
| 4     1 |         | 6     0 |                    
+---------+---------+---------+---------+---------+
|         | 6  1  8 |         | 5     4 |       0 |
|         |         |         |         |         |
| 1       | 4     3 |         | 2  1  8 |         |
+---------+---------+---------+---------+---------+
                    | 8     7 |         | 6  0  1 |
                    |       5 |    0    | 7       |
                    | 4     0 |         | 8     5 |
                    +---------+---------+---------+
                    |         | 6  7  5 |         |
                    |         |         |         |
                    | 2       | 0     1 |       8 |
                    +---------+---------+---------+
+---------+---------+---------+                    
| 0  6  7 | 3  4  2 | 8  1  5 |                    
| 8  5  4 | 7  0  1 | 2  6  3 |                    
| 3  1  2 | 5  8  6 | 0  4  7 |                    
+---------+---------+---------+                    
| 7  3  5 | 8  6  0 | 4  2  1 |                    
| 2  0  6 | 1  7  4 | 3  5  8 |                    
| 4  8  1 | 2  3  5 | 6  7  0 |                    
+---------+---------+---------+---------+---------+
| 5  4  0 | 6  1  8 | 7  3  2 | 5  6  4 | 1  8  0 |
| 6  2  3 | 0  5  7 | 1  8  4 | 7  3  0 | 2  5  6 |
| 1  7  8 | 4  2  3 | 5  0  6 | 2  1  8 | 4  7  3 |
+---------+---------+---------+---------+---------+
                    | 8  2  7 | 4  5  3 | 6  0  1 |
                    | 3  1  5 | 8  0  6 | 7  2  4 |
                    | 4  6  0 | 1  2  7 | 8  3  5 |
                    +---------+---------+---------+
                    | 0  4  8 | 6  7  5 | 3  1  2 |
                    | 6  5  1 | 3  8  2 | 0  4  7 |
                    | 2  7  3 | 0  4  1 | 5  6  8 |
                    +---------+---------+---------+
61 microseconds
guesscount: 0
$
```

- The problem represented with following tokens separated by spaces or line breaks. 
  - (All numbers are in decimal.)
  - Number of sub-Sudokus
  - Number of shared sub-grids.
  - Information about shared sub-grids.
	- `a b c d` means that the sub-grid `b` of sub-Sudoku `a` is shared with the sub-grid `d` of sub-Sudoku `c`.
	- There are 0-indexed.
  - Sub-Sudoku problems.
- Above problem is represented as follow.
```
2 1
0 8 1 0
1 - - 4 - 3 - - 6 - - - - - - - - - - - - 6 9 7 - - - 8 - 6 - - - 5 - 2 - - 7 - 8 - 4 - - 5 - 2 - - - 7 - 1 - - - 7 2 9 - - - - - - - - - - - - 2 - - 5 - 4 - - - 
- - - 6 - 5 - - 1 - - - - - - - - - - - - 3 2 9 - - - 9 - 8 - - - 7 1 2 - - 6 - 1 - 8 - - 5 - 1 - - - 9 - 6 - - - 7 8 6 - - - - - - - - - - - - 3 - - 1 - 2 - - 9 
```

## Author
- Mugi Noda (void-hoge)

## License
- GPLv3
