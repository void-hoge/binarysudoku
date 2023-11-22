#include <iostream>
#include <chrono>
#include "solver.hpp"

int main() {
	auto bd = Board<3>();
	auto slv = Solver<3>();
	bd.input();
	bd.show();
	auto begin = std::chrono::system_clock::now();
	slv.solve(bd, false);
	auto end = std::chrono::system_clock::now();
	if (slv.solutioncount) {
		for (auto&& sol: slv.solutions) {
			sol.show();
		}
	}
	std::cout << std::dec;
	std::cout << "guesses: " << slv.guesscount << std::endl;
	std::cout << "updates: " << slv.updatecount << std::endl;
	std::cout << "elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " microseconds" << std::endl;
	return 0;
}
