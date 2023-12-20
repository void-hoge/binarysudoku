#pragma once

#include "board.hpp"
#include "solver.hpp"

template<uint32_t size, uint32_t algomask = ALL>
class Generator {
	static constexpr auto sqsize = size*size;
	static constexpr auto cbsize = size*sqsize;
	static constexpr auto sqsqsize = sqsize*sqsize;
	std::mt19937 mt;
public:
	Solver<size> slv;
	Board<size> bd;
	std::deque<std::pair<uint32_t, std::uint8_t>> clues;
	Generator(unsigned seed=0);
	void reconstruct();
	bool place_random_clue();
	void init_clues(uint32_t num_clues);
	bool minimize_candidates();
	std::string to_string() const;
};
