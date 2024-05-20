#pragma once

#include "Display.h"
#include "sprites.h"
#include "BlockLogic.h"
#include "Input.h"
#include "Arduino.h"
#include "pitches.h"
#include "kickTables.h"
#include <AlmostRandom.h>

namespace tetris {
	static Matrix<10, 25, char> gameBoard {};
	static int score = 0;

	static const int score_table[4] = {
		120, 300, 900, 3600
	};

	char getRandomBlockType () {
		return "IQTSRLZ"[(random() % 7 + 7) % 7]; // NOLINT(*-msc50-cpp)
	}

	Block getRandomBlock () {
		return { 3, 0, getRandomBlockType(), DIRECTION_U };
	}

	static Block currBlock = getRandomBlock();
	static Block nextBlock = getRandomBlock();
	static Cooldown dropCooldown(4);
	static Cooldown lockCooldown(5);

	template <int8_t displayWidth, int8_t displayHeight>
	void restart_tetris (Display<displayWidth, displayHeight>& display) {
		currBlock = getRandomBlock();
		nextBlock = getRandomBlock();
		gameBoard.fill(0);
		score = 0;

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

		display.matrix().write(next_text, { 12, 8 });
		display.matrix().write(getBlockMatrix(nextBlock.getType()), { 11, 26 });
		redrawScore(display);
		redrawBoard(display);
	}

	template <int8_t displayWidth, int8_t displayHeight>
	void redrawBoard (Display<displayWidth, displayHeight>& display) {
		display.matrix().write(gameBoard, { 0, 7 });

		for (int8_t x = 0; x < 10; x++) {
			if (display.matrix()(x, 11) == 0) display.matrix()(x, 11) = 'D';
		}

		display.update();
	}

	// TODO: refactor
	template <int8_t displayWidth, int8_t displayHeight>
	void checkLines (Display<displayWidth, displayHeight>& display) {
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

				redrawBoard(display);
				delay(200);

				for (int8_t i = 0; i < numClearedLines; i++) {
					memcpy(gameBoard.elements[clearedLines[i]], lineData[i], gameBoard.getWidth());
				}

				redrawBoard(display);
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

					redrawBoard(display);
					delay(100);
				}
			}

			score += score_table[numClearedLines - 1];
		}
	}

	template <int8_t displayWidth, int8_t displayHeight>
	bool checkFail (Display<displayWidth, displayHeight>& display) {
		for (int8_t x = 0; x < 10; x++) {
			if (gameBoard(x, 4) != 0) {
				// Fail
				for (int8_t i = 0; i < 10; i++) {
					display.matrix().write(fail_text_red, { 12, 8 });
					display.update();
					delay(500);
					display.matrix().write(fail_text_white, { 12, 8 });
					display.update();
					delay(500);
				}

				restart_tetris(display);
				return true;
			}
		}
		return false;
	}

	template <int8_t displayWidth, int8_t displayHeight>
	void redrawScore (Display<displayWidth, displayHeight>& display) {

		if (score >= 10000) {
			uint8_t d4 = (score / 1000) % 10;
			uint8_t d5 = (score / 10000) % 10;
			uint8_t d6 = (score / 100000) % 10;
			display.matrix().write(digits[d6], { 0, 0 });
			display.matrix().write(digits[d5], { 4, 0 });
			display.matrix().write(digits[d4], { 8, 0 });
			display.matrix().write(letter_k, { 12, 0 });
		} else {
			uint8_t d1 = score % 10;
			uint8_t d2 = (score / 10) % 10;
			uint8_t d3 = (score / 100) % 10;
			uint8_t d4 = (score / 1000) % 10;
			display.matrix().write(digits[d4], { 0, 0 });
			display.matrix().write(digits[d3], { 4, 0 });
			display.matrix().write(digits[d2], { 8, 0 });
			display.matrix().write(digits[d1], { 12, 0 });
		}

		pinMode(2, OUTPUT);
	}

    void tryKicks(const vec kickData[], Rotation direction){
        if (currBlock.tryRotate(gameBoard, direction)){
            return;
        }
        for (int i = 0; i < 5; i++){
            vec offset;
            if (direction.get() == ROTATE_90_CW.get()){
                offset = kickData[i];
            }
            else {
                offset = vec(-kickData[i].x(), -kickData[i].y());
            }
            if (currBlock.tryApplyModification(
                    gameBoard,
                    offset,
                    direction)){
                return;
            }
        }
    }

	void tryDoRotate (Rotation direction) {
        Rotation curr_rotation = currBlock.rotation;
        switch (curr_rotation.get()){
            case DIRECTION_U.get():
                tryKicks(upToRightKickData, direction);
                return;
            case DIRECTION_R.get():
                tryKicks(rightToDownKickData, direction);
                return;
            case DIRECTION_D.get():
                tryKicks(downToLeftKickData, direction);
                return;
            case DIRECTION_L.get():
                tryKicks(leftToUpKickData, direction);
                return;
        }
	}

	void tryDoMoveSide (Rotation direction) {
		if (currBlock.tryMove(gameBoard, direction)) {
			lockCooldown.reset();
		}
	}

	template <int8_t displayWidth, int8_t displayHeight>
	void tryDoMoveDown (boolean isSoftDrop, Display<displayWidth, displayHeight>& display) {
		if (currBlock.tryMove(gameBoard, DIRECTION_D)) {
			dropCooldown.reset();
			lockCooldown.reset();

			if (isSoftDrop) score++;
		} else if (isSoftDrop || lockCooldown.isFinished()) {
			checkLines(display);
			checkFail(display);

			currBlock = nextBlock;
			nextBlock = getRandomBlock();

			currBlock.writeToMatrix(gameBoard);

			display.matrix().write(getBlockMatrix(nextBlock.getType()), { 11, 26 });

			dropCooldown.reset();
			lockCooldown.reset();
		}
	}

	template <int8_t displayWidth, int8_t displayHeight>
	void tetris_loop (Display<displayWidth, displayHeight>& display) {
		if (input::isJoystickUp()) {
			tryDoRotate(ROTATE_90_CW);
		} else if (input::isJoystickLeft()) {
			tryDoMoveSide(DIRECTION_L);
		} else if (input::isJoystickRight()) {
			tryDoMoveSide(DIRECTION_R);
		}

		if (dropCooldown.isFinished()) {
			tryDoMoveDown(false, display);
		} else if (input::isJoystickDown()) {
			tryDoMoveDown(true, display);
		}

		dropCooldown.tick();
		lockCooldown.tick();

		redrawScore(display);
		redrawBoard(display);

		delay(100);
	}
}