#pragma once

#include <RGBmatrixPanel.h>
#include "Vec.h"
#include "BlockLogic.h"
#include "sprites.h"
#include "util.h"
#include "matrix.h"

template <int8_t width, int8_t height>
class Display {
	uint16_t color_palette[256];

public:
	using matrix_t = Matrix<width, height, char>;

	Display (pin clock, pin outputEnable, pin latch, pin lineSelectorA, pin lineSelectorB, pin lineSelectorC) : panel(
		lineSelectorA.get(), lineSelectorB.get(), lineSelectorC.get(),
		clock.get(), latch.get(), outputEnable.get(),
		false
	) {
		panel.setRotation(1);
	}

	void setColorInPalette(uint8_t c, uint16_t color) {
		color_palette[c] = color;
	}

	template<typename T>
	void setColorsInPalette(T arg) {
		setColorInPalette(arg.first, arg.second);
	}

	template<typename T, typename ...Args>
	void setColorsInPalette(T arg, Args... args) {
		setColorInPalette(arg.first, arg.second);
		setColorsInPalette(args...);
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
				char color_char = matrix_(x, y);

				uint8_t color_index = reinterpret_cast<uint8_t&>(color_char);

				panel.drawPixel(x, y, color_palette[color_index]);
			}
		}
	}

private:
	RGBmatrixPanel panel;
	matrix_t matrix_;
};