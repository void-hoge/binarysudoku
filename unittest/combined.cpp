#include <chrono>

#include "../combined/combined_board.hpp"
#include "../combined/combined_solver.hpp"

constexpr uint32_t size = 3;

int main() {
	int n, m;
	std::cin >> n >> m;
	std::vector<std::array<std::set<std::pair<uint32_t, uint32_t>>, size*size>> links(n);
	for (int i = 0; i < m; i++) {
		int x, y, p, q;
		std::cin >> x >> p >> y >> q;
		links[x][p].emplace(y, q);
		links[y][q].emplace(x, p);
	}
	std::vector<Board<size>> bds(n);
	for (int i = 0; i < n; i++) {
		bds[i].input(std::cin);
	}
	auto cbd = CombinedBoard<size>(bds, links);
	auto cslv = CombinedSolver<size, ALL>();
	cbd.show();
	auto begin = std::chrono::system_clock::now();
	cslv.solve(cbd);
	auto end = std::chrono::system_clock::now();
	for (auto& solution: cslv.solutions) {
		solution.show();
	}
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " microseconds" << std::endl;
	std::cout << "guesscount: " << cslv.guesscount << std::endl;
}
