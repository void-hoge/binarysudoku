#pragma once

#include "board.hpp"

template<uint32_t size>
class Solver {
public:
	uint32_t guesscount;
	uint32_t updatecount;
	uint32_t solutioncount;
	std::vector<Board<size>> solutions;
	bool dfs(Board<size> bd, uint32_t pos, uint8_t num, bool fullsearch);
	void solve(Board<size> bd, bool fullsearch=false);
	bool dfs_check_uniqueness(Board<size> bd, uint32_t pos, uint8_t num);
	bool check_uniqueness(Board<size> bd);
};

