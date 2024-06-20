#include <iostream>
#include "../solver.hpp"

constexpr uint32_t size = 3;

int main() {
	auto bd = Board<size>();
	auto slv = Solver<size>();
	std::vector<int> prob(size * size * size * size);
	for (auto&& num: prob) {
		std::cin >> num;
		num = num == 0 ? size * size : num - 1;
	}
	bd.input(prob);
	slv.solve(bd, false);
	try {
		auto sol = slv.solutions.at(0).to_vector();
		std::cerr << "guesscount: " << slv.guesscount << std::endl;
		std::cerr << "updatecont: " << slv.updatecount << std::endl;
		std::cerr << "solutions : " << slv.solutions.size() << std::endl;
		for (uint32_t i = 0; i < size * size * size * size; i += size * size) {
			for (uint32_t j = 0; j < size * size; j++) {
				std::cout << sol[i + j] << " ";
			}
			std::cout << std::endl;
		}
	}catch (std::range_error& e){}
}
