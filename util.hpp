#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include <bitset>
#include <utility>
#include <tuple>
#include <cassert>
#include <string>
#include <cstdint>
#include <exception>
#include <set>
#include <map>
#include <bit>
#include <random>
#include <queue>

inline std::ostream& operator<<(std::ostream& ost, std::uint8_t num) {
	ost << (int)num;
	return ost;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& ost, const std::vector<T>& vec) {
	ost << "[";
	for (uint32_t i = 0; i < vec.size(); i++) {
		ost << vec[i] << (i == vec.size()-1 ? "" : ", ");
	}
	ost << "]";
	return ost;
}
