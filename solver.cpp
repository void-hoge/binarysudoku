#include "solver.hpp"

template class Solver<3>;
template class Solver<4>;

template<uint32_t size>
bool Solver<size>::dfs(Board<size> bd, uint32_t pos, std::uint8_t num, bool fullsearch) {
	this->guesscount++;
	if (!bd.put(pos, num)) {
		return false;
	}
	while (bd.update()) {
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
	for (auto&& cand: candidates) {
		if (dfs(bd, nextpos, cand, fullsearch)) {
			if (!fullsearch) {
				return true;
			}
		} else {
			bd.erase_single_candidate(nextpos, cand);
		}
	}
	return false;
}

template<uint32_t size>
void Solver<size>::solve(Board<size> bd, bool fullsearch) {
	this->solutions.clear();
	this->guesscount = 0;
	this->updatecount = 0;
	this->solutioncount = 0;
	while (bd.update()) {
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
	for (auto&& cand: candidates) {
		if (this->dfs(bd, nextpos, cand, fullsearch)) {
			if (!fullsearch) {
				return;
			}
		}else {
			bd.erase_single_candidate(nextpos, cand);
			while (bd.update()) {
				this->updatecount++;
			}
			if (!bd.is_valid()) {
				return;
			}
 		}
	}
	if (this->solutioncount == 0) {
		std::cerr << "Invalid Problem. It's cannot be solved." << std::endl;
	}
}

template<uint32_t size>
bool Solver<size>::dfs_check_uniqueness(Board<size> bd, uint32_t pos, uint8_t num) {
	this->guesscount++;
	if (!bd.put(pos, num)) {
		return false;
	}
	while (bd.update()) {
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
	for (auto&& cand: candidates) {
		if (this->dfs_check_uniqueness(bd, nextpos, cand)) {
			solvable = true;
		}
		bd.erase_single_candidate(nextpos, cand);
		if (this->solutioncount > 1) {
			return true;
		}
	}
	return solvable;
}

template<uint32_t size>
bool Solver<size>::check_uniqueness(Board<size> bd) {
	this->guesscount = 0;
	this->updatecount = 0;
	this->solutioncount = 0;
	while (bd.update()) {
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
	for (auto&& cand: candidates) {
		this->dfs_check_uniqueness(bd, nextpos, cand);
		bd.erase_single_candidate(nextpos, cand);
		if (this->solutioncount > 1) {
			return false;
		}
	}
	return this->solutioncount == 1;
}
