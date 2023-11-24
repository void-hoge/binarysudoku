#include "solver.hpp"

template class Solver<3, ALL>;
template class Solver<3, EXCLUDE_SUBSET>;
template class Solver<4, ALL>;
template class Solver<4, EXCLUDE_SUBSET>;

template<uint32_t size, uint32_t algomask>
bool Solver<size, algomask>::dfs(Board<size> bd, uint32_t pos, std::uint8_t num, bool fullsearch) {
	this->guesscount++;
	if (!bd.put(pos, num)) {
		return false;
	}
	while (bd.template update<algomask>()) {
		this->updatecount++;
	}
	if (!bd.is_valid()) {
		return false;
	}
	if (bd.is_solved()) {
		this->solutioncount++;
		this->solutions.push_back(bd);
		return true;
	}
	auto nextpos = bd.get_most_stable_blank();
	auto candidates = bd.get_candidates(nextpos);
	for (uint32_t cand = 0; cand < sqsize; cand++) {
		if (candidates[cand]) {
			if (this->dfs(bd, nextpos, cand, fullsearch)) {
				if (!fullsearch) {
					return true;
				}
			}else {
				bd.erase_single_candidate(nextpos, cand);
			}
		}
	}
	return false;
}

template<uint32_t size, uint32_t algomask>
void Solver<size, algomask>::solve(Board<size> bd, bool fullsearch) {
	this->solutions.clear();
	this->guesscount = 0;
	this->updatecount = 0;
	this->solutioncount = 0;
	while (bd.template update<algomask>()) {
		this->updatecount++;
	}
	if (!bd.is_valid()) {
		std::cerr << "Invalid Problem. It's cannot be solved." << std::endl;
		return;
	}
	if (bd.is_solved()) {
		this->solutioncount++;
		this->solutions.push_back(bd);
		return;
	}
	auto nextpos = bd.get_most_stable_blank();
	auto candidates = bd.get_candidates(nextpos);
	for (uint32_t num = 0; num < sqsize; num++) {
		if (candidates[num]) {
			if (this->dfs(bd, nextpos, num, fullsearch)) {
				if (!fullsearch) {
					return;
				}
			}else {
				bd.erase_single_candidate(nextpos, num);
				while (bd.template update<algomask>()) this->updatecount++;
				if (!bd.is_valid()) return;
			}
		}
	}
	if (this->solutioncount == 0) {
		std::cerr << "Invalid Problem. It's cannot be solved." << std::endl;
	}
}

template<uint32_t size, uint32_t algomask>
bool Solver<size, algomask>::dfs_check_uniqueness(Board<size> bd, uint32_t pos, uint8_t num) {
	this->guesscount++;
	if (!bd.put(pos, num)) {
		return false;
	}
	while (bd.template update<algomask>()) {
		this->updatecount++;
	}
	if (!bd.is_valid()) {
		return false;
	}
	if (bd.is_solved()) {
		this->solutioncount++;
		return true;
	}
	auto nextpos = bd.get_most_stable_blank();
	auto candidates = bd.get_candidates(nextpos);
	bool solvable = false;
	for (uint32_t cand = 0; cand < sqsize; cand++) {
		if (candidates[cand]) {
			if (this->dfs_check_uniqueness(bd, nextpos, cand)) {
				solvable = true;
			}
			bd.erase_single_candidate(nextpos, cand);
			if (this->solutioncount > 1) return true;
		}
	}
	return solvable;
}

template<uint32_t size, uint32_t algomask>
bool Solver<size, algomask>::check_uniqueness(Board<size> bd) {
	this->guesscount = 0;
	this->updatecount = 0;
	this->solutioncount = 0;
	while (bd.template update<algomask>()) {
		this->updatecount++;
	}
	if (!bd.is_valid()) {
		return false;
	}
	if (bd.is_solved()) {
		// unique solution
		this->solutioncount++;
		return true;
	}
	auto nextpos = bd.get_most_stable_blank();
	auto candidates = bd.get_candidates(nextpos);
	for (uint32_t cand = 0; cand < sqsize; cand++) {
		if (candidates[cand]) {
			this->dfs_check_uniqueness(bd, nextpos, cand);
			bd.erase_single_candidate(nextpos, cand);
			if (this->solutioncount > 1) return false;
		}
	}
	return this->solutioncount == 1;
}
