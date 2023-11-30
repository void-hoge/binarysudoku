#include "combined_solver.hpp"

template class CombinedSolver<3, ALL>;
template class CombinedSolver<4, ALL>;

template<uint32_t size, uint32_t algomask>
bool CombinedSolver<size, algomask>::dfs(CombinedBoard<size> bd, uint32_t idx, uint32_t pos, uint8_t num, bool fullsearch) {
	if (!bd.bds[idx].put(pos, num)) {
		return false;
	}
	while (bd.template update<algomask>());
	if (!bd.is_valid()) {
		return false;
	}
	if (bd.is_solved()) {
		this->solutioncount++;
		this->solutions.push_back(bd);
		return true;
	}
	auto [nextidx, nextpos] = bd.get_most_stable_blank();
	auto candidates = bd.bds[nextidx].get_candidates(nextpos);
	for (uint32_t cand = 0; cand < sqsize; cand++) {
		if (candidates[cand]) {
			this->guesscount++;
			if (this->dfs(bd, nextidx, nextpos, cand, fullsearch)) {
				if (!fullsearch) {
					return true;
				}
			}else {
				bd.bds[nextidx].erase_single_candidate(nextpos, cand);
			}
		}
	}
	return false;
}

template<uint32_t size, uint32_t algomask>
void CombinedSolver<size, algomask>::solve(CombinedBoard<size> bd, bool fullsearch) {
	this->guesscount = 0;
	this->solutioncount = 0;
	this->solutions.clear();
	while (bd.template update<algomask>());
	if (!bd.is_valid()) {
		std::cerr << "Invalid Problem. It's cannot be solved." << std::endl;
		return;
	}
	if (bd.is_solved()) {
		this->solutions.push_back(bd);
		return;
	}
	auto [nextidx, nextpos] = bd.get_most_stable_blank();
	auto candidates = bd.bds[nextidx].get_candidates(nextpos);
	for (uint32_t cand = 0; cand < sqsize; cand++) {
		if (candidates[cand]) {
			this->guesscount++;
			if (this->dfs(bd, nextidx, nextpos, cand, fullsearch)) {
				if (!fullsearch) {
					return;
				}
			}else {
				bd.bds[nextidx].erase_single_candidate(nextpos, cand);
				while (bd.template update<algomask>());
				if (!bd.is_valid()) return;
			}
		}
	}
	if (this->solutions.size() == 0) {
		std::cerr << "Invalid Problem. It's cannot be solved." << std::endl;
	}
}
