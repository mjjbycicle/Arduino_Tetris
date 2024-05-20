#include "Display.h"
#include "BlockLogic.h"
#include "Arduino.h"
#include <AlmostRandom.h>
#include "Tetris.h"

Display<16, 32> display(pin { 11 }, pin { 9 }, pin { 10 }, pin { A0 }, pin { A1 }, pin { A2 });

void setup () {
	display.setup();

	display.setColorsInPalette(
		// Display colors
		pair<char, uint16_t>{'W', 0xFFFF},
		pair<char, uint16_t>{'X', 0x2104},
		pair<char, uint16_t>{'D', 0x2000},

		// Tetris colors
		pair<char, uint16_t>{'S', 0xFFE0},
		pair<char, uint16_t>{'Q', 0x065F},
		pair<char, uint16_t>{'I', 0x07D3},
		pair<char, uint16_t>{'R', 0xEc84},
		pair<char, uint16_t>{'T', 0xf86e},
		pair<char, uint16_t>{'L', 0x881f},
		pair<char, uint16_t>{'Z', 0x3B5C},

		// Mushroom mania colors (todo)
		pair<char, uint16_t>{1, 0x0000},
		pair<char, uint16_t>{2, 0x0000},
		pair<char, uint16_t>{3, 0x0000},
		pair<char, uint16_t>{4, 0x0000},
		pair<char, uint16_t>{5, 0x0000}
	);

	randomSeed(AlmostRandom {}.getRandomULong());

	tetris::restart_tetris(display);
}

void loop () {
	tetris::tetris_loop(display);
}