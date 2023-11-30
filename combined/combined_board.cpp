#include "combined_board.hpp"

template class CombinedBoard<3>;
template bool CombinedBoard<3>::update<ALL>();
template class CombinedBoard<4>;
template bool CombinedBoard<4>::update<ALL>();

template<uint32_t size>
uint32_t CombinedBoard<size>::block2pos(uint32_t blkidx, uint32_t inblkidx) const {
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
CombinedBoard<size>::CombinedBoard(
	std::vector<Board<size>>& bds,
	std::vector<std::array<std::set<std::pair<uint32_t, uint32_t>>, sqsize>>& links): bds(bds), links(links) {}

template<uint32_t size>
void CombinedBoard<size>::show(std::ostream& ost) const {
	for (uint32_t i = 0; i < this->bds.size(); i++) {
		for (uint32_t j = 0; j < sqsize; j++) {
			for (auto& [p, q]: links[i][j]) {
				ost << "(" << i << "," << j << ")" << " -> (" << p << "," << q << ")" << std::endl;
			}
		}
	}
	for (auto&& bd: this->bds) {
		bd.show(ost);
	}
}

template<uint32_t size>
uint32_t CombinedBoard<size>::get_candidate_count() const {
	uint32_t ans = 0;
	for (auto&& bd: this->bds) {
		ans += bd.get_candidate_count();
	}
	return ans;
}

template<uint32_t size>
uint32_t CombinedBoard<size>::copy_block(const std::pair<uint32_t, uint32_t> src, const std::pair<uint32_t, uint32_t> dst) {
	auto [sidx, sblk] = src;
	auto [didx, dblk] = dst;
	auto srcblk = this->bds[sidx].get_block_candidates(sblk);
	uint32_t cnt = 0;
	for (uint32_t num = 0; num < sqsize; num++) {
		for (uint32_t inblkidx = 0; inblkidx < sqsize; inblkidx++) {
			if (!srcblk[num][inblkidx]) {
				cnt += this->bds[didx].erase_single_candidate(this->block2pos(dblk, inblkidx), num);
			}
		}
	}
	return cnt;
}

template<uint32_t size>
bool CombinedBoard<size>::is_valid() const {
	bool valid = true;
	for (auto&& bd: this->bds) {
		valid &= bd.is_valid();
	}
	return valid;
}

template<uint32_t size>
bool CombinedBoard<size>::is_solved() const {
	bool solved = true;
	for (auto&& bd: this->bds) {
		solved &= bd.is_solved();
	}
	return solved;
}

template<uint32_t size>
template<uint32_t algomask>
bool CombinedBoard<size>::update_single(const uint32_t idx) {
	auto before = this->bds[idx].get_candidate_count();
	while (this->bds[idx].template update<algomask>());
	auto after = this->bds[idx].get_candidate_count();
	return after < before;
}

template<uint32_t size>
template<uint32_t algomask>
bool CombinedBoard<size>::update() {
	auto before = this->get_candidate_count();
	if (before == this->bds.size() * sqsqsize) return false;
	std::queue<uint32_t> que;
	std::vector<bool> used(this->bds.size(), false);
	for (uint32_t i = 0; i < this->bds.size(); i++) {
		que.push(i);
		used[i] = true;
	}
	while (que.size()) {
		uint32_t idx = que.front();
		que.pop();
		used[idx] = false;
		if (this->update_single<algomask>(idx)) {
			for (uint32_t blk = 0; blk < sqsize; blk++) {
				for (auto&& dst: this->links[idx][blk]) {
					if (this->copy_block({idx, blk}, dst) > 0 and !used[dst.first]) {
						que.push(dst.first);
						used[dst.first] = true;
					}
				}
			}
		}
	}
	auto after = this->get_candidate_count();
	return after < before;
}

template<uint32_t size>
std::pair<uint32_t, uint32_t> CombinedBoard<size>::get_most_stable_blank() const {
	auto [pos, val] = this->bds[0].get_most_stable_blank();
	uint32_t idx = 0;
	for (uint32_t i = 1; i < this->bds.size(); i++) {
		auto tmp = this->bds[i].get_most_stable_blank();
		if (val > tmp.second) {
			pos = tmp.first;
			val = tmp.second;
			idx = i;
		}
	}
	return {idx, pos};
}
