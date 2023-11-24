#include "board.hpp"

template class Board<3>;
template class Board<4>;

template<uint32_t size>
void Board<size>::dump_fullbits(std::ostream& ost, const fullbits& grids) const {
	ost << std::noboolalpha;
	for (uint32_t row = 0; row < sqsize; row++) {
		for (uint8_t num = 0; num < sqsize; num++) {
			for (uint32_t col = 0; col < sqsize; col++) {
				ost << grids[num][row * sqsize + col];
			}
			ost << " ";
		}
		ost << std::endl;
	}
}

template<uint32_t size>
inline uint32_t Board<size>::blockidx(uint32_t pos) const {
	assert(pos < sqsqsize);
	return this->blockidx(pos / sqsize, pos % sqsize);
}

template<uint32_t size>
inline uint32_t Board<size>::blockidx(uint32_t row, uint32_t col) const {
	assert(row < sqsize);
	assert(col < sqsize);
	auto blkrow = row / size;
	auto blkcol = col / size;
	return blkrow * size + blkcol;
}

template<uint32_t size>
inline uint32_t Board<size>::inblockidx(uint32_t pos) const {
	assert(pos < sqsqsize);
	return this->inblockidx(pos / sqsize, pos % sqsize);
}

template<uint32_t size>
inline uint32_t Board<size>::inblockidx(uint32_t row, uint32_t col) const {
	assert(row < sqsize);
	assert(col < sqsize);
	auto inblkrow = row % size;
	auto inblkcol = col % size;
	return inblkrow * size + inblkcol;
}

template<uint32_t size>
inline uint32_t Board<size>::block2pos(uint32_t blkidx, uint32_t inblkidx) const {
	assert(blkidx < sqsize);
	assert(inblkidx < sqsize);
	auto blkrow = blkidx / size;
	auto blkcol = blkidx % size;
	auto inblkrow = inblkidx / size;
	auto inblkcol = inblkidx % size;
	auto row = blkrow * size + inblkrow;
	auto col = blkcol * size + inblkcol;
	return row * sqsize + col;
}

template<uint32_t size>
uint32_t Board<size>::fullbits_popcount(const fullbits& grid) const {
	uint32_t count = 0;
	for (auto&& tmp: grid) {
		count += tmp.count();
	}
	return count;
}

template<uint32_t size>
Board<size>::bits Board<size>::fullbits_union(const fullbits& grid) const {
	bits ret;
	for (auto&& tmp: grid) {
		ret |= tmp;
	}
	return ret;
}

template<uint32_t size>
Board<size>::bits Board<size>::get_naked_singles(const fullbits& grid) const {
	auto once = bits(0);
	auto ret = bits(0).flip();
	for (uint32_t i = 1; i < sqsize; i++) {
		once = once | grid[i-1];
		ret = ~(ret & grid[i] & once) & ret;
	}
	return (once | grid.back()) & ret;
}

template<uint32_t size>
void Board<size>::update_naked_single() {
	auto naked_singles = this->get_naked_singles(this->candidates);
	for (uint32_t row = 0; row < sqsize; row++) {
		for (uint32_t col = 0; col < sqsize; col++) {
			auto pos = row * sqsize + col;
			if (naked_singles[pos]) {
				for (uint32_t num = 0; num < sqsize; num++) {
					if (this->put(pos, num)) {
						break;
					}
				}
			}
		}
	}
}

template<uint32_t size>
void Board<size>::update_hidden_single() {
	for (uint8_t num = 0; num < sqsize; num++) {
		for (uint32_t row = 0; row < sqsize; row++) {
			if (!this->stablerows[row][num]) {
				auto mask = this->candidates[num] & this->rowmask(row);
				if (mask.count() == 1) {
					for (uint32_t col = 0; col < sqsize; col++) {
						if (mask[row * sqsize + col]) {
							this->put(row, col, num);
							break;
						}
					}
				}
			}
		}
		for (uint32_t col = 0; col < sqsize; col++) {
			if (!this->stablecols[col][num]) {
				auto mask = this->candidates[num] & this->colmask(col);
				if (mask.count() == 1) {
					for (uint32_t row = 0; row < sqsize; row++) {
						if (mask[row * sqsize + col]) {
							this->put(row, col, num);
							break;
						}
					}
				}
			}
		}
		for (uint32_t blkidx = 0; blkidx < sqsize; blkidx++) {
			if (!this->stableblocks[blkidx][num]) {
				auto mask = this->candidates[num] & this->blockmask(blkidx);
				if (mask.count() == 1) {
					for (uint32_t inblkidx = 0; inblkidx < sqsize; inblkidx++) {
						auto pos = this->block2pos(blkidx, inblkidx);
						if (mask[pos]) {
							this->put(pos, num);
							break;
						}
					}
				}
			}
		}
	}
}

template<uint32_t size>
void Board<size>::update_locked_candidate() {
	for (uint32_t num = 0; num < sqsize; num++) {
		for (uint32_t blkidx = 0; blkidx < sqsize; blkidx++) {
			auto block = this->candidates[num] & this->blockmask(blkidx);
			auto blkrow = blkidx / size;
			auto blkcol = blkidx % size;

			for (uint32_t i = 0; i < size; i++) {
				if (!this->stablerows[blkrow * size + i][num]) {
					auto row = this->candidates[num] & this->rowmask(blkrow * size + i);
					if (!this->stableblocks[blkidx][num] and
						(~row & block).none()) { // pointing
						auto mask = row ^ block;
						this->candidates[num] &= ~mask;
					}
					if ((~block & row).none()) { // claiming
						auto mask = row ^ block;
						this->candidates[num] &= ~mask;
					}
				}
			}

			for (uint32_t i = 0; i < size; i++) {
				if (!this->stablecols[blkcol * size + i][num]) {
					auto col = this->candidates[num] & this->colmask(blkcol * size + i);
					if (!this->stableblocks[blkidx][num] and
						(~col & block).none()) { // pointing
						auto mask = col ^ block;
						this->candidates[num] &= ~mask;
					}
					if ((~block & col).none()) { // claiming
						auto mask = col ^ block;
						this->candidates[num] &= ~mask;
					}
				}
			}
		}
	}
}

template<uint32_t size>
void Board<size>::update_hidden_row_subset(uint8_t limit) {
	for (uint32_t row = 0; row < sqsize; row++) {
		for (uint32_t num = 0; num < sqsize-1; num++) {
			if (this->stablerows[row][num]) continue;
			auto mask = this->candidates[num] & this->rowmask(row);
			auto ppc = mask.count();
			if (not(1 < ppc and ppc <= limit)) continue;
			auto cnt = ppc-1;
			std::vector<uint8_t> nums(1, num);
			for (uint32_t n = num+1; n < sqsize and cnt > 0; n++) {
				if ((this->candidates[n] & this->rowmask(row)) == mask) {
					nums.push_back(n);
					cnt--;
				}
			}
			if (cnt == 0) {
				for (uint32_t i = 0; i < sqsize; i++) {
					this->candidates[i] &= ~mask;
				}
				for (auto i: nums) {
					this->candidates[i] |= mask;
				}
			}
		}
	}
}

template<uint32_t size>
void Board<size>::update_hidden_col_subset(uint8_t limit) {
	for (uint32_t col = 0; col < sqsize; col++) {
		for (uint32_t num = 0; num < sqsize-1; num++) {
			if (this->stablecols[col][num]) continue;
			auto mask = this->candidates[num] & this->colmask(col);
			auto ppc = mask.count();
			if (not(1 < ppc and ppc <= limit)) continue;
			auto cnt = ppc-1;
			std::vector<uint8_t> nums(1, num);
			for (uint32_t n = num+1; n < sqsize and cnt > 0; n++) {
				if ((this->candidates[n] & this->colmask(col)) == mask) {
					nums.push_back(n);
					cnt--;
				}
			}
			if (cnt == 0) {
				for (uint32_t i = 0; i < sqsize; i++) {
					this->candidates[i] &= ~mask;
				}
				for (auto i: nums) {
					this->candidates[i] |= mask;
				}
			}
		}
	}
}

template<uint32_t size>
void Board<size>::update_hidden_block_subset(uint8_t limit) {
	for (uint32_t blkidx = 0; blkidx < sqsize; blkidx++) {
		for (uint32_t num = 0; num < sqsize-1; num++) {
			if (this->stableblocks[blkidx][num]) continue;
			auto mask = this->candidates[num] & this->blockmask(blkidx);
			auto ppc = mask.count();
			if (not(1 < ppc and ppc <= limit)) continue;
			auto cnt = ppc-1;
			std::vector<uint8_t> nums(1, num);
			for (uint32_t n = num+1; n < sqsize and cnt > 0; n++) {
				if ((this->candidates[n] & this->blockmask(blkidx)) == mask) {
					nums.push_back(n);
					cnt--;
				}
			}
			if (cnt == 0) {
				for (uint32_t i = 0; i < sqsize; i++) {
					this->candidates[i] &= ~mask;
				}
				for (auto i: nums) {
					this->candidates[i] |= mask;
				}
			}
		}
	}
}

template<uint32_t size>
void Board<size>::update_hidden_subset(uint8_t limit) {
	this->update_hidden_row_subset(limit);
	this->update_hidden_col_subset(limit);
	this->update_hidden_block_subset(limit);
}

template<uint32_t size>
void Board<size>::update_naked_subset(uint8_t limit) {
	std::array<std::bitset<sqsize>, sqsqsize> cellcands;
	std::fill(cellcands.begin(), cellcands.end(), 0);
	for (uint8_t num = 0; num < sqsize; num++) {
		for (uint32_t pos = 0; pos < sqsqsize; pos++) {
			cellcands[pos] |= this->candidates[num][pos] << num;
		}
	}
	for (uint32_t row = 0; row < sqsize; row++) {
		for (uint32_t col = 0; col < sqsize; col++) {
			auto pos = row*sqsize+col;
			auto cands = cellcands[pos];
			auto ppc = cands.count();
			if (not(1 < ppc and ppc <= limit)) continue;
			auto basemask = bits(0);
			basemask[row*sqsize+col] = true;
			// same row
			{
				auto mask = basemask;
				auto cnt = ppc-1;
				for (uint32_t c = col+1; c < sqsize and cnt > 0; c++) {
					if (cands == cellcands[row*sqsize+c]) {
						mask[row*sqsize+c] = true;
						cnt--;
					}
				}
				if (cnt == 0) {
					for (uint8_t num = 0; num < sqsize; num++) {
						if (cands[num]) {
							this->candidates[num] &= ~(this->rowmask(row) ^ mask);
						}
					}
				}
			}
			// same col
			{
				auto mask = basemask;
				auto cnt = ppc-1;
				for (uint32_t r = row+1; r < sqsize and cnt > 0; r++) {
					if (cands == cellcands[r*sqsize+col]) {
						mask[r*sqsize+col] = true;
						cnt--;
					}
				}
				if (cnt == 0) {
					for (uint8_t num = 0; num < sqsize; num++) {
						if (cands[num]) {
							this->candidates[num] &= ~(this->colmask(col) ^ mask);
						}
					}
				}
			}
			// same block
			{
				auto blkidx = this->blockidx(row, col);
				auto inblkidx = this->inblockidx(row, col);
				auto mask = basemask;
				auto cnt = ppc-1;
				for (uint32_t i = inblkidx+1; i < sqsize and cnt > 0; i++) {
					if (cands == cellcands[this->block2pos(blkidx, i)]) {
						mask[this->block2pos(blkidx, i)] = true;
						cnt--;
					}
				}
				if (cnt == 0) {
					for (uint8_t num = 0; num < sqsize; num++) {
						if (cands[num]) {
							this->candidates[num] &= ~(this->blockmask(blkidx) ^ mask);
						}
					}
				}
			}
		}
	}
}

template<uint32_t size>
Board<size>::Board() : rowmask_(rowmaskgen()), colmask_(colmaskgen()), blockmask_(blockmaskgen()){
	this->init_stables();
	this->init_candidates();
}

template<uint32_t size>
Board<size>::Board(const Board<size>& bd) : rowmask_(rowmaskgen()), colmask_(colmaskgen()), blockmask_(blockmaskgen()){
	this->stable = bd.stable;
	this->stablerows = bd.stablerows;
	this->stablecols = bd.stablecols;
	this->stableblocks = bd.stableblocks;
	this->candidates = bd.candidates;
}

template<uint32_t size>
Board<size>& Board<size>::operator=(const Board<size>& bd) {
	this->stable = bd.stable;
	this->stablerows = bd.stablerows;
	this->stablecols = bd.stablecols;
	this->stableblocks = bd.stableblocks;
	this->candidates = bd.candidates;
	return *this;
}

template<uint32_t size>
void Board<size>::dump(std::ostream& ost) const {
	ost << "candidates" << std::endl;
	this->dump_fullbits(ost, this->candidates);
}

template<uint32_t size>
void Board<size>::show(std::ostream& ost) const {
	ost << std::hex;
	std::stringstream ss;
	for (uint32_t i = 0; i < sqsize; i++) {
		if ((i % size) == 0) {
			ss << "+";
		}
		ss << "---";
	}
	ss << "+";
	auto line = ss.str();
	for (uint32_t row = 0; row < sqsize; row++) {
		if (row % size == 0) {
			ost << line << std::endl;
		}
		for (uint32_t col = 0; col < sqsize; col++) {
			if (col % size == 0) {
				ost << "|";
			}
			auto tmp = this->stable[row][col];
			if (tmp == sqsize) {
				ost << "   ";
			}else {
				ost << " " << tmp << " ";
			}
		}
		ost << "|" << std::endl;
	}
	ost << line << std::endl;
	ost << std::dec;
}

template<uint32_t size>
void Board<size>::input(std::istream& ist) {
	for (uint32_t row = 0; row < sqsize; row++) {
		for (uint32_t col = 0; col < sqsize; col++) {
			std::string tmp;
			ist >> tmp;
			if (tmp == "-") {
				continue;
			}else {
				try {
					this->put(row, col, std::stoi(tmp)-1);
				}catch (std::exception& e) {}
			}
		}
	}
}

template<uint32_t size>
void Board<size>::input(std::vector<int>& problem) {
	for (uint32_t row = 0; row < sqsize; row++) {
		for (uint32_t col = 0; col < sqsize; col++) {
			auto pos = row * sqsize + col;
			if (problem[pos] != sqsize) {
				this->put(row, col, problem[pos]);
			}
		}
	}
}

template<uint32_t size>
bool Board<size>::put(uint32_t row, uint32_t col, uint8_t num) {
	assert(row < sqsize);
	assert(col < sqsize);
	assert(num < sqsize);
	if (this->stable[row][col] != sqsize) {
		return true;
	}
	auto pos = row * sqsize + col;
	if (!this->candidates[num][pos]) {
		return false;
	}
	auto blkidx = this->blockidx(row, col);
	this->stable[row][col] = num;
	this->stablerows[row][num] = true;
	this->stablecols[col][num] = true;
	this->stableblocks[blkidx][num] = true;
	this->candidates[num] &= ~this->rowmask(row);
	this->candidates[num] &= ~this->colmask(col);
	this->candidates[num] &= ~this->blockmask(blkidx);
	for (auto&& val: this->candidates) {
		val[pos] = false;
	}
	this->candidates[num][pos] = true;
	return true;
}

template<uint32_t size>
bool Board<size>::put(uint32_t pos, uint8_t num) {
	assert(pos < sqsqsize);
	assert(num < sqsize);
	auto row = pos / sqsize;
	auto col = pos % sqsize;
	return this->put(row, col, num);
}

template<uint32_t size>
bool Board<size>::is_valid() const {
	for (uint32_t num = 0; num < sqsize; num++) {
		for (uint32_t i = 0; i < sqsize; i++) {
			if ((this->candidates[num] & this->rowmask(i)).none()) {
				return false;
			}
			if ((this->candidates[num] & this->colmask(i)).none()) {
				return false;
			}
			if ((this->candidates[num] & this->blockmask(i)).none()) {
				return false;
			}
		}
	}
	return this->fullbits_popcount(this->candidates) >= sqsqsize and this->fullbits_union(this->candidates).all();
}

template<uint32_t size>
bool Board<size>::is_solved() const {
	return this->fullbits_popcount(this->candidates) == sqsqsize and this->fullbits_union(this->candidates).all();
}

template<uint32_t size>
Board<size>::bits Board<size>::get_blank() const {
	return this->get_naked_singles(this->candidates).flip();
}

template<uint32_t size>
uint32_t Board<size>::get_most_stable_blank() const {
	std::array<uint8_t, sqsqsize> blankcnt;
	std::fill(blankcnt.begin(), blankcnt.end(), 0);
	for (uint32_t num = 0; num < sqsize; num++) {
		for (uint32_t pos = 0; pos < sqsqsize; pos++) {
			blankcnt[pos] += this->candidates[num][pos];
		}
	}
	auto min = sqsize+1;
	auto idx = sqsqsize;
	for (uint32_t pos = 0; pos < sqsqsize; pos++) {
		if (blankcnt[pos] > 1) {
			if (blankcnt[pos] < min) {
				min = blankcnt[pos];
				idx = pos;
			}
		}
	}
	return idx;
}

template<uint32_t size>
std::bitset<Board<size>::sqsize> Board<size>::get_candidates(uint32_t pos) const {
	std::bitset<sqsize> cands;
	for (uint32_t num = 0; num < sqsize; num++) {
		if (this->candidates[num][pos]) {
			cands[num] = 1;
		}
	}
	return cands;
}

template<uint32_t size>
std::bitset<Board<size>::sqsize> Board<size>::get_candidates(uint32_t row, uint32_t col) const {
	return this->get_candidates(row*sqsize + col);
}

template<uint32_t size>
uint32_t Board<size>::get_candidate_count() const {
	return this->fullbits_popcount(this->candidates);
}

template<uint32_t size>
void Board<size>::erase_single_candidate(uint32_t pos, uint32_t num) {
	this->candidates[num][pos] = false;
}

template<uint32_t size>
void Board<size>::erase_single_candidate(uint32_t row, uint32_t col, uint32_t num) {
	this->candidates[num][row*sqsize+col] = false;
}

template<uint32_t size>
void Board<size>::erase_stable(uint32_t pos) {
	this->erase_stable(pos/sqsize, pos%sqsize);
}

template<uint32_t size>
void Board<size>::erase_stable(uint32_t row, uint32_t col) {
	auto num = this->stable[row][col];
	if (num == sqsize) {
		return;
	}
	auto blkidx = this->blockidx(row, col);
	this->stable[row][col] = sqsize;
	this->stablerows[row][num] = false;
	this->stablecols[col][num] = false;
	this->stableblocks[blkidx][num] = false;
}

template<uint32_t size>
void Board<size>::init_stables() {
	for (auto&& row: this->stable) {
		for (auto&& val: row) {
			val = sqsize;
		}
	}
	for (auto&& val: this->stablerows) {
		val = std::bitset<sqsize>(0);
	}
	for (auto&& val: this->stablecols) {
		val = std::bitset<sqsize>(0);
	}
	for (auto&& val: this->stableblocks) {
		val = std::bitset<sqsize>(0);
	}
}

template<uint32_t size>
void Board<size>::init_candidates() {
	for (auto&& grid: this->candidates) {
		grid = bits(0).flip();
	}
}

template<uint32_t size>
std::string Board<size>::to_string() const {
	std::stringstream ss;
	ss << std::dec;
	for (auto&& line: this->stable) {
		for (auto&& num: line) {
			if (num == sqsize) {
				ss << "- ";
			}else {
				ss << num << " ";
			}
		}
	}
	return ss.str();
}
