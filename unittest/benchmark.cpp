#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <cassert>
#include "../board.hpp"
#include "../solver.hpp"

constexpr uint32_t size = 3;

int main(const int argc, const char** argv) {
	assert(argc == 2);
	std::string filename(argv[1]);
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		throw std::invalid_argument("cannot open "+filename);
	}
	std::string buf;
	uint32_t problem_count = 0;
	uint32_t guess_count = 0;
	uint32_t no_guess_count = 0;
	uint64_t total_time = 0;
	while (std::getline(ifs, buf)) {
		if (buf == "") continue;
		Board<size> bd;
		Solver<size> slv;
		std::istringstream iss(buf);
		auto begin = std::chrono::system_clock::now();
		bd.input(iss);
		slv.solve(bd, false);
		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		total_time += elapsed;
		problem_count++;
		no_guess_count += slv.guesscount == 0;
		guess_count += slv.guesscount;
		std::cout << std::dec
				  << "No." << problem_count << "\n"
				  << "elapsed: " << (double)elapsed/1000 << " microseconds\n"
				  << std::endl;
	}
	std::cout << "average time: " << (double)total_time/problem_count/1000 << " microseconds\n"
			  << "no guess: " << no_guess_count << " / " << problem_count
			  << " (" << (double)no_guess_count/problem_count*100 << " %)\n"
			  << "average guesses: " << (double)guess_count/problem_count << std::endl;
}
