#pragma once

#include <RGBmatrixPanel.h>
#include "Coordinates.h"
#include "BlockLogic.h"
#include "Blocks.h"
#include "Helpers.h"
#include "util.h"

template <uint8_t width, uint8_t height>
class Display {
	RGBmatrixPanel panel;
	char _matrix[height][width] {};

public:

	Display (pin clock, pin outputEnable, pin latch, pin lineSelectorA, pin lineSelectorB, pin lineSelectorC) : panel(
		lineSelectorA.get(), lineSelectorB.get(), lineSelectorC.get(),
		clock.get(), latch.get(), outputEnable.get(),
		false
	) {
		panel.setRotation(1);
	}

	const char (& (matrix) () const )[height][width] {
		return _matrix;
	}

	char (& (matrix) ())[height][width] {
		return _matrix;
	}

	void setup () {
		panel.begin();
	}

	void update () {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				uint16_t color = getColor(_matrix[y][x]);
				panel.drawPixel(x, y, color);
			}
		}
	}
};