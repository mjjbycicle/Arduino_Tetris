#pragma once

#include <stdint.h>

template <uint8_t width, uint8_t height, typename T>
class Matrix {
	T elements[height][width];

public:
	/// @deprecated
	const T (& (getElements_) () const )[height][width] {
		return elements;
	}

	/// @deprecated
	T (& (getElements_) ())[height][width] {
		return elements;
	}

	const T& operator()(uint8_t x, uint8_t y) const {
		return elements[y][x];
	}

	T& operator()(uint8_t x, uint8_t y) {
		return elements[y][x];
	}
};