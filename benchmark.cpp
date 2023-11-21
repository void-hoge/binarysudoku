#include <iostream>
#include <chrono>
#include <fstream>
#include "board.hpp"
#include "solver.hpp"

void read_text_file(const std::string filename, std::string& data) {
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		throw std::invalid_argument("cannot open "+filename);
	}
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	data = std::string(it, last);
	ifs.close();
}

int main(const int argc, const char** argv) {
	std::string data;
	read_text_file(std::string(argv[1]), data);
	std::stringstream ss(data);
	std::string buf;
	std::vector<std::string> problems;
	while (std::getline(ss, buf)) {
		if (buf == "") continue;
		problems.push_back(buf);
	}
	long long total = 0;
	int count = 0;
	int guesses = 0;
	for (std::size_t i = 0; i < problems.size(); i++) {
		auto problem = problems[i];
		Board<3> bd;
		Solver<3> slv;
		std::stringbuf buf(problem.c_str());
		std::istream ist(&buf);
		bd.input(ist);
		auto begin = std::chrono::system_clock::now();
		slv.solve(bd, false);
//		slv.check_uniqueness(bd);
		auto end = std::chrono::system_clock::now();
		count += slv.guesscount == 0;
		guesses += slv.guesscount;
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
		std::cerr << std::dec
				  << "No." << i << '\n'
				  << "guesses: " << slv.guesscount << "\n"
				  << "time: " << (double)elapsed/1000 << " microseconds" << std::endl << std::endl;
		if (slv.solutioncount == 0) {
			std::cout << "error" << std::endl;
			return -1;
		}
				
		total += elapsed;
	}
	std::cout << "average time: " << (double)total/problems.size()/1000 << " microseconds" << std::endl;
	std::cout << "no guess count: "
			  << count << " / " << problems.size()
			  << " (" <<  (double)count/problems.size()*100 << "%)" << std::endl;
	std::cout << "average guesses: "
			  << (double)guesses/problems.size() << std::endl;
	
}
