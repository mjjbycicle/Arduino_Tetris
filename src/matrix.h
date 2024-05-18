#pragma once

#include <stdint.h>

template <int8_t width, int8_t height, typename T>
class Matrix {
	T elements[height][width];

public:

	const T& operator()(uint8_t x, uint8_t y) const {
		return elements[y][x];
	}

	T& operator()(uint8_t x, uint8_t y) {
		return elements[y][x];
	}
};