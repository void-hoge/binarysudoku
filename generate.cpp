#include <iostream>
#include <chrono>
#include "generator.hpp"

constexpr uint32_t size = 3;

#define RANDOM_SEED

int main() {
	uint32_t clues = 17;
	uint32_t attempts = 0;
	uint32_t seed = 0;
#if defined(RANDOM_SEED)
	std::random_device rng;
	seed = rng();
	std::cout << "seed: " << seed << std::endl;
	Generator<size> gen(seed);
#else
	std::cin >> seed;
	std::cout << "seed: " << seed << std::endl;
	Generator<size> gen(seed);
#endif
	std::cout << "clues: " << clues << std::endl;
	std::cout << "attempting" << std::flush;
	auto begin = std::chrono::system_clock::now();
	do {
		std::cout << "." << std::flush;
		attempts++;
	} while (!gen.generate(clues));
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	std::cout << std::endl;
	gen.reconstruct();
	gen.bd.show();
	gen.slv.solve(gen.bd, true);
	gen.slv.solutions.front().show();
	std::cout << attempts << " attempts in " << (double)elapsed/1000000000 << " seconds" << std::endl;
	std::cout << "solution count: " << gen.slv.solutions.size() << std::endl;
	std::cout << "guess count: " << gen.slv.guesscount << std::endl;
}
