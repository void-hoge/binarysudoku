#pragma once

#include "combined_board.hpp"

template<uint32_t size, uint32_t algomask>
class CombinedSolver {
private:
	// aliases
	static constexpr auto sqsize = size*size;
	static constexpr auto cbsize = size*sqsize;
	static constexpr auto sqsqsize = sqsize*sqsize;
public:
	uint32_t guesscount;
	uint32_t solutioncount;
	std::vector<CombinedBoard<size>> solutions;

	bool dfs(CombinedBoard<size> bd, uint32_t idx, uint32_t pos, uint8_t num, bool fullsearch);
	void solve(CombinedBoard<size> bd, bool fullsearch = true);
};
