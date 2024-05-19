#include "Display.h"
#include "Game.h"
#include "sprites.h"
#include "BlockLogic.h"
#include "Input.h"
#include "Arduino.h"
#include <AlmostRandom.h>

Display<16, 32> display(pin { 11 }, pin { 9 }, pin { 10 }, pin { A0 }, pin { A1 }, pin { A2 });
Matrix<10, 25, char> gameBoard;
int score = 0;

const int score_table[4] = {
	120, 300, 900, 3600
};

char getRandomBlockType () {
	return "IQTSRLZ"[(random() % 7 + 7) % 7]; // NOLINT(*-msc50-cpp)
}

Block getRandomBlock () {
	return { 3, 0, getRandomBlockType(), DIRECTION_U };
}

Block currBlock = getRandomBlock();
Block nextBlock = getRandomBlock();
Cooldown dropCooldown(4);
Cooldown lockCooldown(5);

void redrawBoard () {
	display.matrix().write(gameBoard, { 0, 7 });

	for (int8_t x = 0; x < 10; x++) {
		if (display.matrix()(x, 11) == 0) display.matrix()(x, 11) = 'D';
	}

	display.update();
}

void checkFail () {
	for (int8_t x = 0; x < 10; x++) {
		if (gameBoard(x, 4) != 0) {
			// Fail
			delay(2000);
			gameBoard.fill(0);
			score = 0;

			nextBlock = getRandomBlock();
			currBlock = getRandomBlock();

			display.matrix().write(getBlockMatrix(nextBlock.type), {11, 26});
		}
	}
}

void checkLines () {
	int8_t numClearedLines = 0;
	static int8_t clearedLines[4];
	for (int8_t y = gameBoard.getHeight() - 1; y >= 0; y--) {
		// Check if clearedLines is filled
		bool isLineFilled = true;
		for (int8_t x = 0; x < gameBoard.getWidth(); x++) {
			if (!gameBoard(x, y)) {
				isLineFilled = false;
				break;
			}
		}
		if (isLineFilled) {
			clearedLines[numClearedLines++] = y;
		}
		if (numClearedLines == 4) break;
	}

	if (numClearedLines != 0) {
		// Flashing animation

		// Save clearedLines colors
		static char lineData[4][gameBoard.getWidth()];
		for (int8_t i = 0; i < numClearedLines; i++) {
			memcpy(lineData[i], gameBoard.elements[clearedLines[i]], gameBoard.getWidth());
		}
		// Flash 4 times
		for (int8_t i = 0; i < 4; i++) {
			for (int8_t i = 0; i < numClearedLines; i++) {
				memset(gameBoard.elements[clearedLines[i]], 'W', gameBoard.getWidth());
			}

			redrawBoard();
			delay(200);

			for (int8_t i = 0; i < numClearedLines; i++) {
				memcpy(gameBoard.elements[clearedLines[i]], lineData[i], gameBoard.getWidth());
			}

			redrawBoard();
			delay(200);
		}

		// Update clearedLines. Remember that clearedLines is sorted greatest to least
		int8_t currClearedLineIndex = 0;

		for (int8_t destLine = gameBoard.getHeight() - 1, srcLine = destLine; srcLine >= 0; destLine--, srcLine--) {
			while (currClearedLineIndex < numClearedLines && srcLine == clearedLines[currClearedLineIndex]) {
				memset(gameBoard.elements[srcLine], 0, gameBoard.getWidth());
				srcLine--;
				currClearedLineIndex++;
			}

			if (srcLine >= 0 && srcLine != destLine) {
				memcpy(gameBoard.elements[destLine], gameBoard.elements[srcLine], gameBoard.getWidth());
				memset(gameBoard.elements[srcLine], 0, gameBoard.getWidth());

				// check if line is zero
				{
					bool is_zero = true;
					for (int8_t x = 0; x < gameBoard.getWidth(); x++) {
						if (gameBoard.elements[destLine][x] != 0) {
							is_zero = false;
							break;
						}
					}
					if (is_zero) {
						break;
					}
				}

				redrawBoard();
				delay(100);
			}
		}

		score += score_table[numClearedLines - 1];
	}
}

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
	currBlock = getRandomBlock();
	nextBlock = getRandomBlock();

	// Draw lines
	for (int8_t x = 0; x < display.matrix().getWidth(); x++) {
		display.matrix()(x, 6) = 'X';
	}
	for (int8_t x = 11; x < display.matrix().getWidth(); x++) {
		display.matrix()(x, 24) = 'X';
	}
	for (int8_t y = 6; y < display.matrix().getHeight(); y++) {
		display.matrix()(10, y) = 'X';
	}

	display.matrix().write(next_text, {12, 8});
	display.matrix().write(getBlockMatrix(nextBlock.type), {11, 26});
}

void redrawScore () {

	if(score >= 10000) {
		uint8_t d4 = (score / 1000) % 10;
		uint8_t d5 = (score / 10000) % 10;
		uint8_t d6 = (score / 100000) % 10;
		display.matrix().write(digits[d6], {0, 0});
		display.matrix().write(digits[d5], {4, 0});
		display.matrix().write(digits[d4], {8, 0});
		display.matrix().write(letter_k, {12, 0});
	}
	else {
		uint8_t d1 = score % 10;
		uint8_t d2 = (score / 10) % 10;
		uint8_t d3 = (score / 100) % 10;
		uint8_t d4 = (score / 1000) % 10;
		display.matrix().write(digits[d4], {0, 0});
		display.matrix().write(digits[d3], {4, 0});
		display.matrix().write(digits[d2], {8, 0});
		display.matrix().write(digits[d1], {12, 0});
	}
}

void loop () {
	if (input::isJoystickUp()) {
		if ((input::isJoystickLeft() && currBlock.tryApplyModification(gameBoard, {}, rotateCCW(currBlock.rotation))) ||
			(input::isJoystickRight() && currBlock.tryApplyModification(gameBoard, {}, rotateCW(currBlock.rotation)))) {
			lockCooldown.reset();
		}
	} else {
		if (input::isJoystickLeft() && currBlock.tryApplyModification(gameBoard, getOffset(DIRECTION_L), currBlock.rotation)) {
			lockCooldown.reset();
		}
		if (input::isJoystickRight() && currBlock.tryApplyModification(gameBoard, getOffset(DIRECTION_R), currBlock.rotation)) {
			lockCooldown.reset();
		}
	}

	if(dropCooldown.isFinished()) {
		if(currBlock.tryApplyModification(gameBoard, getOffset(DIRECTION_D), currBlock.rotation)) {
			dropCooldown.reset();
			lockCooldown.reset();
		}
		else if(lockCooldown.isFinished()) {
			checkLines();
			checkFail();

			currBlock = nextBlock;
			nextBlock = getRandomBlock();

			currBlock.writeToMatrix(gameBoard);

			display.matrix().write(getBlockMatrix(nextBlock.type), {11, 26});

			dropCooldown.reset();
			lockCooldown.reset();
		}
	}
	else if (input::isJoystickDown() || input::isJoystickClicked()) {
		if(currBlock.tryApplyModification(gameBoard, getOffset(DIRECTION_D), currBlock.rotation)) {
			dropCooldown.reset();
			lockCooldown.reset();
			score++;
		}
		else {
			checkLines();
			checkFail();

			currBlock = nextBlock;
			nextBlock = getRandomBlock();

			currBlock.writeToMatrix(gameBoard);

			display.matrix().write(getBlockMatrix(nextBlock.type), {11, 26});

			dropCooldown.reset();
			lockCooldown.reset();
		}
	}

	dropCooldown.tick();
	lockCooldown.tick();

	redrawScore();
	redrawBoard();
	delay(100);
}
