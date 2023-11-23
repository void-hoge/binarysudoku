#pragma once

#include "util.hpp"

template<uint32_t size>
class Board {
private:
	// aliases
	static constexpr auto sqsize = size*size;
	static constexpr auto cbsize = size*sqsize;
	static constexpr auto sqsqsize = sqsize*sqsize;
	using bits = std::bitset<sqsqsize>;
	using fullbits = std::array<bits, sqsize>;

	// data
	std::array<std::array<uint8_t, sqsize>, sqsize> stable;
	std::array<std::bitset<sqsize>, sqsize> stablerows;
	std::array<std::bitset<sqsize>, sqsize> stablecols;
	std::array<std::bitset<sqsize>, sqsize> stableblocks;
	fullbits candidates;

	// masks
	const bits rowmask_ = []() {
		auto rowmask = bits(0);
		for (uint32_t i = 0; i < sqsize; i++) {
			rowmask[i] = true;
		}
		return rowmask;
	}();
	const bits colmask_ = []() {
		auto colmask = bits(0);
		for (uint32_t i = 0; i < sqsize; i++) {
			colmask[i*sqsize] = true;
		}
		return colmask;
	}();
	const bits blockmask_ = []() {
		auto blockmask = bits(0);
		for (uint32_t i = 0; i < size; i++) {
			for (uint32_t j = 0; j < size; j++) {
				blockmask[i*sqsize+j] = true;
			}
		}
		return blockmask;
	}();

	inline bits rowmask(uint32_t row) const {
		return this->rowmask_ << (row*sqsize);
	}

	inline bits colmask(uint32_t col) const {
		return this->colmask_ << col;
	}

	inline bits blockmask(uint32_t blkidx) const {
		auto blkrow = blkidx/size;
		auto blkcol = blkidx%size;
		return this->blockmask_ << (blkrow * cbsize + blkcol * size);
	}

	void dump_fullbits(std::ostream& ost, const fullbits& grids) const;

	// coordinate converters
	inline uint32_t blockidx(uint32_t pos) const;
	inline uint32_t blockidx(uint32_t row, uint32_t col) const;
	inline uint32_t inblockidx(uint32_t pos) const;
	inline uint32_t inblockidx(uint32_t row, uint32_t col) const;
	inline uint32_t block2pos(uint32_t blkidx, uint32_t inblkidx) const;

	// algorithmics
	uint32_t fullbits_popcount(const fullbits& grid) const;
	bits fullbits_union(const fullbits& grid) const;
	bits get_naked_singles(const fullbits& grid) const;
	void update_hidden_single();
	void update_naked_single();
	void update_locked_candidates();
	void update_hidden_row_subset(uint8_t limit=size);
	void update_hidden_col_subset(uint8_t limit=size);
	void update_hidden_block_subset(uint8_t limit=size);
	void update_hidden_subset(uint8_t limit=size);
	void update_naked_subset(uint8_t limit=size);
public:
	Board();
	void dump(std::ostream& ost = std::cout) const;
	void show(std::ostream& ost = std::cout) const;
	void input(std::istream& ist = std::cin);
	void input(std::vector<int>& problem);
	bool put(uint32_t row, uint32_t col, uint8_t num);
	bool put(uint32_t pos, uint8_t num);
	bool update();
	bool is_valid() const;
	bool is_solved() const;
	bits get_blank() const;
	uint32_t get_most_stable_blank() const;
	std::vector<uint8_t> get_candidates(uint32_t pos) const;
	std::vector<uint8_t> get_candidates(uint32_t row, uint32_t col) const;
	void erase_single_candidate(uint32_t pos, uint32_t num);
	void erase_single_candidate(uint32_t row, uint32_t col, uint32_t num);
	void erase_stable(uint32_t pos);
	void erase_stable(uint32_t row, uint32_t col);
	void reset_stables();
	void reset_candidates();
};
