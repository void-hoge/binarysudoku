#include "generator.hpp"

template class Generator<3, ALL>;
template class Generator<3, EXCLUDE_SUBSET>;
template class Generator<4, ALL>;
template class Generator<4, EXCLUDE_SUBSET>;

template<uint32_t size, uint32_t algomask>
Generator<size, algomask>::Generator(uint32_t seed) : mt(std::mt19937(seed)) {}

template<uint32_t size, uint32_t algomask>
void Generator<size, algomask>::reconstruct() {
	this->bd.init_stables();
	this->bd.init_candidates();
	for (auto&& [pos, num]: this->clues) {
		this->bd.put(pos, num);
	}
}

template<uint32_t size, uint32_t algomask>
bool Generator<size, algomask>::place_random_clue() {
	this->reconstruct();
	auto blank = this->bd.get_blank();
	uint32_t pos = 0;
	if (blank.count() == 0) {
		pos = this->mt() % sqsqsize;
	}else {
		uint32_t idx = this->mt() % blank.count();
		for (uint32_t i = 0, cnt = 0; i < sqsqsize; i++, cnt += blank[i]) {
			if (blank[i] and cnt == idx) {
				pos = i;
				break;
			}
		}
	}
	auto candidates = this->bd.get_candidates(pos);
	uint8_t num = 0;
	uint8_t idx = this->mt() % candidates.count();
	for (uint32_t i = 0, cnt = 0; i < sqsize; i++, cnt += candidates[i]) {
		if (candidates[i] and cnt == idx) {
			num = i;
			break;
		}
	}
	this->reconstruct();
	this->bd.put(pos, num);
	while (this->bd.update());
	bool valid = this->bd.is_valid();
	if (valid) {
		this->clues.push_back({pos, num});
	}
	return valid;
}

template<uint32_t size, uint32_t algomask>
void Generator<size, algomask>::init_clues(uint32_t num_clues) {
	this->clues.clear();
	this->reconstruct();
	while (this->clues.size() < num_clues) this->place_random_clue();
}

template<uint32_t size, uint32_t algomask>
bool Generator<size, algomask>::minimize_candidates() {
	const uint32_t num_clues = this->clues.size();
	this->reconstruct();
	while (this->bd.template update<algomask>());
	uint32_t current = this->bd.get_candidate_count();
	for (uint32_t i = 0; i <= num_clues and current > sqsqsize; i++) {
		auto minimum = this->clues.front();
		this->clues.pop_front();
		this->reconstruct();
		while (this->bd.template update<algomask>());
		auto blank = this->bd.get_blank();
		uint32_t min_evalue = current;
		Board<size> backup = bd;
		for (uint32_t pos = 0; pos < sqsqsize; pos++) {
			if (blank[pos]) {
				auto candidates = this->bd.get_candidates(pos);
				for (uint32_t num = 0; num < sqsize; num++) {
					if (candidates[num]) {
						this->bd = backup;
						this->bd.put(pos, num);
						while (this->bd.template update<algomask>());
						if (this->bd.is_valid()) {
							auto evalue = this->bd.get_candidate_count();
							if (evalue < min_evalue) {
								minimum = {pos, num};
								min_evalue = evalue;
							}
						}
					}
				}
			}
		}
		this->clues.push_back(minimum);
		if (current > min_evalue) {
			i = 0;
		}
		current = min_evalue;
	}
	this->reconstruct();
	return this->slv.check_uniqueness(this->bd);
}

template<uint32_t size, uint32_t algomask>
std::string Generator<size, algomask>::to_string() const {
	std::array<uint8_t, sqsqsize> grid;
	std::fill(grid.begin(), grid.end(), sqsize);
	for (auto&& [pos, num]: this->clues) {
		grid[pos] = num;
	}
	std::stringstream ss;
	for (auto&& num: grid) {
		if (num == sqsize) {
			ss << "-";
		}else {
			ss << num + 1;
		}
		ss << " ";
	}
	return ss.str();
}
