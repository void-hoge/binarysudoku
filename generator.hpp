#pragma once

template<std::size_t size>
class Generator{
public:
	static constexpr auto sqsize = size*size;
	static constexpr auto cbsize = size*sqsize;
	static constexpr auto sqsqsize = sqsize*sqsize;
	std::mt19937 mt;
	Solver<size> slv;
	Board<size> bd;
	std::vector<std::pair<std::size_t, std::uint8_t>> cells;
	Generator(unsigned seed=0);
	void reconstruct();
	bool put_random();
};

template<std::size_t size>
Generator<size>::Generator(unsigned seed = 0) : mt(std::mt19937(seed)) {
	for (auto&& cell: this->cells) {
		cell = 0;
	}
}

template<std::size_t size>
void Generator<size>::reconstruct() {
	this->bd.reset_stables();
	this->bd.reset_candidates();
	for (auto [pos, num]: this->cells) {
		this->bd.put(pos, num);
	}
}

template<std::size_t size>
bool Generator<size>::put_random() {
	auto blank = this->bd.get_blank();
	if (blank.count() == 0) {
		return true;
	}
	std::vector<std::size_t> blanks;
	for (std::size_t i = 0; i < sqsqsize; i++) {
		if (blank[i]) {
			blanks.push_back(i);
		}
	}
	auto pos = blanks[this->mt()%blanks.size()];
	auto cands = this->bd.get_candidates(pos);
	auto num = cands[this->mt()%cands.size()];
	this->bd.put(pos, num);
	while (this->bd.update());
	if (!this->bd.check_error()) {
		this->reconstruct();
		while (this->bd.update());
		return false;
	}
	this->cells.push_back({pos, num});
	return true;
}
