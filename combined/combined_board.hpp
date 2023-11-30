#pragma once

#include "../board.hpp"

template<uint32_t size>
class CombinedBoard {
public:
	static constexpr auto sqsize = size*size;
	static constexpr auto cbsize = size*sqsize;
	static constexpr auto sqsqsize = sqsize*sqsize;
	uint32_t block2pos(uint32_t blkidx, uint32_t inblkidx) const;
	std::vector<Board<size>> bds;
	const std::array<std::array<std::set<std::pair<uint32_t, uint32_t>>, sqsize>, sqsize> links;
	CombinedBoard(
		std::vector<Board<size>>& bds,
		std::array<std::array<std::set<std::pair<uint32_t, uint32_t>>, sqsize>, sqsize>& links);

	uint32_t get_candidate_count() const;
	uint32_t copy_block(
		const std::pair<uint32_t, uint32_t> src,
		const std::pair<uint32_t, uint32_t> dst);

	bool is_valid() const;
	bool is_solved() const;

	template<uint32_t algomask = ALL>
	bool update_single(const uint32_t idx);

	template<uint32_t algomask = ALL>
	bool update(const int32_t start = -1);

	void show(std::ostream& ost = std::cout) const;
	std::pair<uint32_t, uint32_t> get_most_stable_blank() const;
};
