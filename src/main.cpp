#include "Display.h"
#include "BlockLogic.h"
#include "Arduino.h"
#include <AlmostRandom.h>
#include "Tetris.h"

Display<16, 32> display(pin { 11 }, pin { 9 }, pin { 10 }, pin { A0 }, pin { A1 }, pin { A2 });

void setup () {
	display.setup();
	display.setColorInPalette('W', 0xFFFF);
	display.setColorInPalette('X', 0x2104);
	display.setColorInPalette('D', 0x2000);
	display.setColorInPalette('Z', 0x3B5C);
	display.setColorInPalette('S', 0xFFE0);
	display.setColorInPalette('Q', 0x065F);
	display.setColorInPalette('I', 0x07D3);
	display.setColorInPalette('R', 0xEc84);
	display.setColorInPalette('L', 0x881f);
	display.setColorInPalette('T', 0xf86e);

	randomSeed(AlmostRandom {}.getRandomULong());

	tetris::restart_tetris(display);
}

void loop () {
	tetris::tetris_loop(display);
}