#pragma once

#include <RGBmatrixPanel.h>
#include "Coordinates.h"
#include "BlockLogic.h"
#include "Blocks.h"
#include "Helpers.h"
#include "util.h"
#include "matrix.h"

template <int8_t width, int8_t height>
class Display {
public:
	using matrix_t = Matrix<width, height, char>;

	Display (pin clock, pin outputEnable, pin latch, pin lineSelectorA, pin lineSelectorB, pin lineSelectorC) : panel(
		lineSelectorA.get(), lineSelectorB.get(), lineSelectorC.get(),
		clock.get(), latch.get(), outputEnable.get(),
		false
	) {
		panel.setRotation(1);
	}

	const matrix_t& matrix() const {
		return matrix_;
	}

	matrix_t& matrix () {
		return matrix_;
	}

	void setup () {
		panel.begin();
	}

	void update () {
		for (int8_t y = 0; y < height; y++) {
			for (int8_t x = 0; x < width; x++) {
				uint16_t color = getColor(matrix_(x, y));
				panel.drawPixel(x, y, color);
			}
		}
	}

private:
	RGBmatrixPanel panel;
	matrix_t matrix_;
};