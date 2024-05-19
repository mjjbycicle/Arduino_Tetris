#include "Display.h"
#include "Game.h"
#include "Blocks.h"
#include "BlockLogic.h"
#include "Input.h"
#include "Arduino.h"
#include <AlmostRandom.h>

Display<16, 32> display(pin{11}, pin{9}, pin{10}, pin{A0}, pin{A1}, pin{A2});
Matrix<10, 25, char> gameBoard;

char getRandomBlockType() {
	return "QSTRILZ"[(random() % 7 + 7) % 7]; // NOLINT(*-msc50-cpp)
}

Block getRandomBlock() {
	return {3, 0, getRandomBlockType(), DIRECTION_U};
}

Block currBlock = getRandomBlock();
Cooldown dropCooldown(4);
Cooldown lockCooldown(4);

void redrawBoard() {
	display.matrix().write(gameBoard, {0, 7});

	for(int8_t x = 0; x < 10; x++) {
		if(display.matrix()(x, 11) == 0) display.matrix()(x, 11) = 'D';
	}

	display.update();
}

void checkFail() {
	for(int8_t x = 0; x < 10; x++) {
		if(gameBoard(x, 4) != 0) {
			// Fail
			delay(2000);
			gameBoard.fill(0);
		}
	}
}

void checkLines() {
	int8_t numClearedLines = 0;
	static int8_t clearedLines[4];
	for(int8_t y = gameBoard.getHeight() - 1; y >= 0; y--) {
		// Check if clearedLines is filled
		bool isLineFilled = true;
		for(int8_t x = 0; x < gameBoard.getWidth(); x++) {
			if (!gameBoard(x, y)) {
				isLineFilled = false;
				break;
			}
		}
		if(isLineFilled) {
			clearedLines[numClearedLines++] = y;
		}
		if(numClearedLines == 4) break;
	}

	if(numClearedLines != 0) {
		// Flashing animation

		// Save clearedLines colors
		static char lineData[4][gameBoard.getWidth()];
		for(int8_t i = 0; i < numClearedLines; i++) {
			memcpy(lineData[i], gameBoard.elements[clearedLines[i]], gameBoard.getWidth());
		}
		// Flash 4 times
		for(int8_t i = 0; i < 4; i++) {
			for(int8_t i = 0; i < numClearedLines; i++) {
				memset(gameBoard.elements[clearedLines[i]], 'W', gameBoard.getWidth());
			}

			redrawBoard();
			delay(200);

			for(int8_t i = 0; i < numClearedLines; i++) {
				memcpy(gameBoard.elements[clearedLines[i]], lineData[i], gameBoard.getWidth());
			}

			redrawBoard();
			delay(200);
		}

		// Update clearedLines. Remember that clearedLines is sorted greatest to least
		int8_t currClearedLineIndex = 0;

		for(int8_t destLine = gameBoard.getHeight() - 1, srcLine = destLine; srcLine >= 0; destLine--, srcLine--) {
			while(currClearedLineIndex < numClearedLines && srcLine == clearedLines[currClearedLineIndex]) {
				memset(gameBoard.elements[srcLine], 0, gameBoard.getWidth());
				srcLine--;
				currClearedLineIndex++;
			}

			if(srcLine >= 0 && srcLine != destLine) {
				memcpy(gameBoard.elements[destLine], gameBoard.elements[srcLine], gameBoard.getWidth());
				memset(gameBoard.elements[srcLine], 0, gameBoard.getWidth());

				// check if line is zero
				{
					bool is_zero = true;
					for(int8_t x = 0; x < gameBoard.getWidth(); x++) {
						if(gameBoard.elements[destLine][x] != 0) {
							is_zero = false;
							break;
						}
					}
					if(is_zero) {
						break;
					}
				}

				redrawBoard();
				delay(100);
			}
		}
	}
}

void setup() {
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

	randomSeed(AlmostRandom{}.getRandomULong());

	// Draw lines
	for(int8_t x = 0; x < display.matrix().getWidth(); x++) {
		display.matrix()(x, 6) = 'X';
	}
	for(int8_t y = 6; y < display.matrix().getHeight(); y++) {
		display.matrix()(10, y) = 'X';
	}
}

void loop() {
	if (input::isJoystickUp()) {
		if (input::isJoystickLeft()) {
			currBlock.eraseFromMatrix(gameBoard);
			currBlock.direction = rotateCCW(currBlock.direction);
			currBlock.writeToMatrix(gameBoard);
			lockCooldown.reset();
		} else if (input::isJoystickRight()) {
			currBlock.eraseFromMatrix(gameBoard);
			currBlock.direction = rotateCW(currBlock.direction);
			currBlock.writeToMatrix(gameBoard);
			lockCooldown.reset();
		}
	}

	if (input::isJoystickLeft() && currBlock.canMoveInDirection(gameBoard, DIRECTION_L)) {
		currBlock.move(gameBoard, DIRECTION_L);
		lockCooldown.reset();
	}
	if (input::isJoystickRight() && currBlock.canMoveInDirection(gameBoard, DIRECTION_R)) {
		currBlock.move(gameBoard, DIRECTION_R);
		lockCooldown.reset();
	}

	if((input::isJoystickDown() || dropCooldown.isFinished()) && currBlock.canMoveInDirection(gameBoard, DIRECTION_D)) {
		currBlock.move(gameBoard, DIRECTION_D);
		dropCooldown.reset();
		lockCooldown.reset();
	}

	if(!currBlock.canMoveInDirection(gameBoard, DIRECTION_D) && lockCooldown.isFinished()) {
		checkLines();
		checkFail();

		currBlock = getRandomBlock();
		currBlock.writeToMatrix(gameBoard);
		dropCooldown.reset();
		lockCooldown.reset();
	}

	dropCooldown.tick();
	lockCooldown.tick();

	redrawBoard();
	delay(100);
}
