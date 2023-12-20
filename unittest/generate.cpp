#include <iostream>
#include <chrono>
#include "../generator.hpp"

template<uint32_t size, uint32_t algomask>
void test(uint32_t clues, uint32_t seed) {
	Generator<size, algomask> gen(seed);
	std::cerr << "seed: " << seed << "\n"
			  << "clues: " << clues << std::endl;
	std::cerr << "attempting" << std::flush;
	uint32_t attempts = 0;
	auto begin = std::chrono::system_clock::now();
	do {
		std::cerr << "." << std::flush;
		attempts++;
		gen.init_clues(clues);
		gen.minimize_candidates();
	} while (!gen.minimize_solutions());
	std::cerr << std::endl;
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	gen.reconstruct();
	std::vector<int> tmp(size*size*size*size, size*size);
	for (auto&& [pos, num]: gen.clues) {
		tmp[pos] = num;
	}
	gen.bd.show(std::cerr);
	gen.slv.solve(gen.bd, true);
	for (auto&& sol: gen.slv.solutions) {
		sol.show(std::cerr);
	}
	std::cout << gen.to_string() << std::endl;
	std::cerr << attempts << " attempts in " << (double)elapsed/1000000000 << " seconds (" << (double)attempts/elapsed*1000000000 << " attempts per second)\n"
			  << "solutions: " << gen.slv.solutions.size() << "\n"
			  << "guesses: " << gen.slv.guesscount << std::endl;
}

int main(const int argc, const char **argv) {
	if (argc != 3 and argc != 4) throw std::invalid_argument("Invalid command line argument.");
	long size = std::stoi(argv[1]);
	int clues = std::stoi(argv[2]);
	if (clues <= 0) {
		throw std::invalid_argument("Invalid clues number.");
	}
	uint32_t seed;
	if (argc == 4) {
		seed = std::stoll(argv[3]);
	}else {
		std::random_device rng;
		seed = rng();
	}
	if (size == 3) {
		test<3, EXCLUDE_SUBSET>(clues, seed);
	}else if (size == 4) {
		test<4, ALL>(clues, seed);
	}else {
		throw std::invalid_argument("Invalid Sudoku size.");
	}
}
