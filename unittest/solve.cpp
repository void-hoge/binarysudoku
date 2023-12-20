#include <iostream>
#include <chrono>
#include <cassert>
#include "../solver.hpp"

template<uint32_t size, uint32_t algomask>
void test() {
	auto bd = Board<size>();
	auto slv = Solver<size, algomask>();
	bd.input();
	bd.show();
	auto begin = std::chrono::system_clock::now();
	slv.solve(bd, /* full_search= */ false, /* savesolution= */ true);
	auto end = std::chrono::system_clock::now();
	for (auto&& sol: slv.solutions) {
		sol.show();
	}
	std::cout << "guesses: " << slv.guesscount << "\n"
			  << "updates: " << slv.updatecount << "\n"
			  << "elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " microseconds" << std::endl;
}

int main(const int argc, const char **argv) {
	if (argc == 1 or (argc == 2 and std::stoi(argv[1]) == 3)) {
		test<3, EXCLUDE_SUBSET>();
	}else if (argc == 2 and std::stoi(argv[1]) == 4) {
		test<4, ALL>();
	}else {
		throw std::invalid_argument("Invalid command line argument.");
	}
}
