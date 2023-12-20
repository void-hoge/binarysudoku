#pragma once

#include "board.hpp"

template<uint32_t size, uint32_t algomask = ALL>
class Solver {
private:
	static constexpr auto sqsize = size*size;
	static constexpr auto cbsize = size*sqsize;
	static constexpr auto sqsqsize = sqsize*sqsize;
public:
	uint32_t guesscount;
	uint32_t updatecount;
	uint32_t solutioncount;
	std::vector<Board<size>> solutions;
	bool dfs(Board<size> bd, uint32_t pos, uint8_t num, bool fullsearch, bool savesolution, uint32_t limit=~0);
	bool solve(Board<size> bd, bool fullsearch=false, bool savesolution=true, uint32_t limit=1<<31);
	bool dfs_check_uniqueness(Board<size> bd, uint32_t pos, uint8_t num);
	bool check_uniqueness(Board<size> bd);
};

