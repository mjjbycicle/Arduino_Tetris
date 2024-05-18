#pragma once

#include "Blocks.h"
#include "Coordinates.h"
#include "Helpers.h"
#include "matrix.h"
#include "util.h"

bool isOccupied (char matrix[24][10], int r, int c);

bool occupiedInBlock (char matrix[5][5], int r, int c);

void rotateNTimes (char (& dest)[5][5], char block[5][5], char type, RotationAmount numRot);

void rotate (char (& dest)[5][5], char block[5][5], char type);

class Block {
private:
	bool isLocked_ { false };
	char type;
	RotationAmount numRot;

public:
	point topRightCorner;

	Block (int8_t x, int8_t y, char type, int numRot) : type(type), topRightCorner(x, y) {
		this->numRot.set(numRot);
	}

	void getDisplayVec (char (& arr)[5][5]) const {
		getRawVec(arr, type);
	}

	char getType () const {
		return type;
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	void writeToMatrix (Matrix<matrixWidth, matrixHeight, char>& matrix) const {
		char currArr[5][5];
		getDisplayVec(currArr);
		for (int dy = 0; dy < 5; dy++) {
			for (int dx = 0; dx < 5; dx++) {
				char currChar = currArr[dy][dx];
				if (currChar) {
					matrix(dx + topRightCorner.x(), dy + topRightCorner.y()) = currChar;
				}
			}
		}
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	void eraseFromMatrix (Matrix<matrixWidth, matrixHeight, char>& matrix) const {
		char currArr[5][5];
		getDisplayVec(currArr);
		for (int dy = 0; dy < 5; dy++) {
			for (int dx = 0; dx < 5; dx++) {
				char currChar = currArr[dy][dx];
				if (currChar) {
					matrix(dx + topRightCorner.x(), dy + topRightCorner.y()) = 0;
				}
			}
		}
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	void move (Matrix<matrixWidth, matrixHeight, char>& matrix, Direction direction) {
		if (canMoveInDirection(matrix, direction)) {
			eraseFromMatrix(matrix);

			topRightCorner.x() += getDeltaX(direction);
			topRightCorner.y() += getDeltaY(direction);

			writeToMatrix(matrix);
		} else if(direction == DIRECTION_D) {
			isLocked_ = true;
		}
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	bool canMoveInDirection (Matrix<matrixWidth, matrixHeight, char>& matrix, Direction direction) {
		char currArr[5][5];
		getDisplayVec(currArr);
		
		int8_t modifierX = getDeltaX(direction);
		int8_t modifierY = getDeltaY(direction);
		
		for (int dy = 0; dy < 5; dy++) {
			for (int dx = 0; dx < 5; dx++) {
				if (currArr[dy][dx] != 0 && isOccupied(matrix, topRightCorner.x() + dx, topRightCorner.y() + dy)) {

					int otherY = dy + modifierY;
					int otherX = dx + modifierX;

					// If this point is in our block
					if(currArr[dy][dx]) {
						// If there is not an occupied neighbor in our block
						if((otherX < 0 || otherX >= 5 || otherY < 0 || otherY >= 5 || !currArr[otherY][otherX])) {
							// But the neighbor point is indeed occupied
							if (isOccupied(matrix, topRightCorner.x() + otherX, topRightCorner.y() + otherY)) {
								return false;
							}
						}
					}
				}
			}
		}
		return true;
	}

	bool isLocked () const {
		return isLocked_;
	}
};


void rotate3X3 (char (& dest)[5][5], char block[5][5]) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			dest[i][j] = block[5 - j - 1][i];
		}
	}
}

void rotateOther (char (& dest)[5][5], char block[5][5]) {
	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 5; c++) {
			if (c == 0 or r == 4) {
				dest[r][c] = 0;
			} else {
				dest[c - 1][5 - r - 1] = block[r][c];
			}
		}
	}
}

void rotate (char (& dest)[5][5], char block[5][5], char type) {
	if (type == 'Q' or type == 'I') {
		rotateOther(dest, block);
	} else {
		rotate3X3(dest, block);
	}
}

void rotateNTimes (char (& dest)[5][5], char block[5][5], char type, RotationAmount numRot) {

}

template <int8_t matrixWidth, int8_t matrixHeight>
bool isOccupied (const Matrix<matrixWidth, matrixHeight, char>& matrix, int x, int y) {
	return y >= 24 || x <= -1 || x >= 10 || matrix(x, y);
}

bool occupiedInBlock (char matrix[5][5], int r, int c) {
	if (matrix[r][c] != 0) return true;
	return false;
}