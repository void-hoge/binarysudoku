#pragma once

#include "util.hpp"

constexpr uint32_t NAKED_SINGLE     = 1 << 0;
constexpr uint32_t HIDDEN_SINGLE    = 1 << 1;
constexpr uint32_t LOCKED_CANDIDATE = 1 << 2;
constexpr uint32_t HIDDEN_SUBSET    = 1 << 3;
constexpr uint32_t NAKED_SUBSET     = 1 << 4;
constexpr uint32_t FISH             = 1 << 5;
constexpr uint32_t ALL              = ~0;
constexpr uint32_t EXCLUDE_SUBSET   = NAKED_SINGLE | HIDDEN_SINGLE | LOCKED_CANDIDATE;

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
	static constexpr auto rowmaskgen = [] {
		auto rowmask = bits(0);
		for (uint32_t i = 0; i < sqsize; i++) {
			rowmask[i] = true;
		}
		return rowmask;
	};
	static constexpr auto colmaskgen = [] {
		auto colmask = bits(0);
		for (uint32_t i = 0; i < sqsize; i++) {
			colmask[i*sqsize] = true;
		}
		return colmask;
	};
	static constexpr auto blockmaskgen = [] {
		auto blockmask = bits(0);
		for (uint32_t i = 0; i < size; i++) {
			for (uint32_t j = 0; j < size; j++) {
				blockmask[i*sqsize+j] = true;
			}
		}
		return blockmask;
	};
	const bits rowmask_;
	const bits colmask_;
	const bits blockmask_;

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
	void update_locked_candidate();
	void update_hidden_row_subset(uint8_t limit=size);
	void update_hidden_col_subset(uint8_t limit=size);
	void update_hidden_block_subset(uint8_t limit=size);
	void update_hidden_subset(uint8_t limit=size);
	void update_naked_subset(uint8_t limit=size);
public:
	Board();
	Board(const Board<size>& bd);
	Board<size>& operator=(const Board<size>& bd);
	void dump(std::ostream& ost = std::cout) const;
	void show(std::ostream& ost = std::cout) const;
	void input(std::istream& ist = std::cin);
	void input(std::vector<int>& problem);
	bool put(uint32_t row, uint32_t col, uint8_t num);
	bool put(uint32_t pos, uint8_t num);

	template<uint32_t algomask = ALL>
	bool update() {
		auto before = this->fullbits_popcount(this->candidates);
		if (before == sqsqsize) {
			this->update_naked_single();
			return false;
		}
		if constexpr(algomask & NAKED_SINGLE    ) this->update_naked_single();
		if constexpr(algomask & HIDDEN_SINGLE   ) this->update_hidden_single();
		if constexpr(algomask & LOCKED_CANDIDATE) this->update_locked_candidate();
		if constexpr(algomask & HIDDEN_SUBSET   ) this->update_hidden_subset();
		if constexpr(algomask & NAKED_SUBSET    ) this->update_naked_subset();
		auto after = this->fullbits_popcount(this->candidates);
		return after != before;
	}
	bool is_valid() const;
	bool is_solved() const;
	bits get_blank() const;
	std::pair<uint32_t, uint32_t> get_most_stable_blank() const;
	std::bitset<sqsize> get_candidates(uint32_t pos) const;
	std::bitset<sqsize> get_candidates(uint32_t row, uint32_t col) const;
	std::array<std::bitset<sqsize>, sqsize> get_block_candidates(uint32_t blkidx) const;
	uint32_t get_candidate_count() const;
	bool erase_single_candidate(uint32_t pos, uint32_t num);
	bool erase_single_candidate(uint32_t row, uint32_t col, uint32_t num);
	void erase_stable(uint32_t pos);
	void erase_stable(uint32_t row, uint32_t col);
	void init_stables();
	void init_candidates();
	std::string to_string() const;
};
