#pragma once

#include <stdint.h>

template <int8_t width, int8_t height, typename T>
struct MatrixInitializer {
	T elements[height][width];
};

template <int8_t width, int8_t height, typename T>
class Matrix {
	static T dummy;

public:
	T elements[height][width];

	constexpr int8_t getWidth () const {
		return width;
	}

	constexpr int8_t getHeight () const {
		return height;
	}

	constexpr bool contains(int x, int y) const {
		return x >= 0 && x < width && y >= 0 && y < height;
	}

	constexpr bool contains(const vec& p) const {
		return contains(p.x(), p.y());
	}

	constexpr const T& operator() (int8_t x, int8_t y) const {
		return contains(x, y) ? elements[y][x] : dummy;
	}

	T& operator() (int8_t x, int8_t y) {
		return contains(x, y) ? elements[y][x] : dummy;
	}

	constexpr const T& operator() (const vec& position) const {
		return (*this)(position.x(), position.y());
	}

	T& operator() (const vec& position) {
		return (*this)(position.x(), position.y());
	}

	void fill (T value) {
		if (sizeof(T) == 1) {
			memset(&elements[0][0], value, width * height);
		} else {
			for (int8_t i = 0; i < width * height; i++) {
				*(&elements[0][0] + i) = value;
			}
		}
	}

	template <int8_t otherWidth, int8_t otherHeight>
	void write (const Matrix<otherWidth, otherHeight, T>& other, vec topLeftCorner) {
		for (int8_t y = 0; y < otherHeight; y++) {
			for (int8_t x = 0; x < otherWidth; x++) {
				(*this)(topLeftCorner + vec{ x, y}) = other(x, y);
			}
		}
	}

	template <int8_t otherWidth, int8_t otherHeight, typename U, typename F>
	void writeMapped (const Matrix<otherWidth, otherHeight, U>& other, vec topLeftCorner, F function) {
		for (int8_t y = 0; y < otherHeight; y++) {
			for (int8_t x = 0; x < otherWidth; x++) {
				(*this)(topLeftCorner + vec{ x, y}) = function(other(x, y));
			}
		}
	}
};

template <int8_t width, int8_t height, typename T>
T Matrix<width, height, T>::dummy {}; // NOLINT(*-dynamic-static-initializers)