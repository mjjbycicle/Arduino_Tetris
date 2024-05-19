#pragma once

#include "Blocks.h"
#include "Coordinates.h"
#include "Helpers.h"
#include "matrix.h"
#include "util.h"

vec getRotatedBlockOffset (__attribute__((unused)) char type, vec p, Direction direction) {
	// Just do rotate3x3 for now
	return
		direction == DIRECTION_L ?
		vec { p.y() + 0, 4 - p.x() } :
		direction == DIRECTION_D ?
		vec { 4 - p.x(), 4 - p.y() } :
		direction == DIRECTION_R ?
		vec { 4 - p.y(), p.x() + 0 } :
		p;
}

vec rotateOffset (vec offset, Direction direction) {
	switch (direction.get()) {
		case DIRECTION_U.get():
			return offset;
		case DIRECTION_L.get():
			return { +offset.y(), -offset.x() };
		case DIRECTION_D.get():
			return { -offset.x(), -offset.y() };
		case DIRECTION_R.get():
			return { -offset.y(), +offset.x() };
		default:
			exit(1);
	}
}

class Block {
private:
	char type;

public:
	Direction direction;
	vec topRightCorner;

	Block (int8_t x, int8_t y, char type, Direction direction) : type(type), direction(direction), topRightCorner(x, y) {}

	template <int8_t matrixWidth, int8_t matrixHeight>
	void writeToMatrix (Matrix<matrixWidth, matrixHeight, char>& matrix) const {
		const BlockMatrix& blockMatrix = getBlockMatrix(type);
		for (int8_t dy = 0; dy < 5; dy++) {
			for (int8_t dx = 0; dx < 5; dx++) {
				char currChar = blockMatrix(dx, dy);
				if (currChar) {
					matrix(topRightCorner + getRotatedBlockOffset(type, { dx, dy }, direction)) = currChar;
				}
			}
		}
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	void eraseFromMatrix (Matrix<matrixWidth, matrixHeight, char>& matrix) const {
		const BlockMatrix& blockMatrix = getBlockMatrix(type);
		for (int dy = 0; dy < 5; dy++) {
			for (int dx = 0; dx < 5; dx++) {
				char currChar = blockMatrix(dx, dy);
				if (currChar) {
					matrix(topRightCorner + getRotatedBlockOffset(type, { dx, dy }, direction)) = 0;
				}
			}
		}
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	bool canMoveInDirection (Matrix<matrixWidth, matrixHeight, char>& matrix, Direction direction) {
//		const BlockMatrix& blockMatrix = getBlockMatrix(type);
//
		int8_t modifierX = getDeltaX(direction);
		int8_t modifierY = getDeltaY(direction);
//
//		for (int8_t dy = 0; dy < 5; dy++) {
//			for (int8_t dx = 0; dx < 5; dx++) {
////				vec currBlockOffset = getRotatedBlockOffset(type, {dx, dy}, Direction{static_cast<int8_t>(direction.get() ^ 1)});
//				vec currBlockOffset = getRotatedBlockOffset(type, {dx, dy}, direction);
//				vec neighborBlockOffset = currBlockOffset + vec {modifierX, modifierY};
//
//				// If this vec is in our block and the neighbor vec is not
//				if (blockMatrix(currBlockOffset) != 0 && !blockMatrix(neighborBlockOffset)) {
//					// But the neighbor vec is indeed occupied (or out of bounds)
//					const vec& neighborPoint = topRightCorner + neighborBlockOffset;
//
//					if (!matrix.contains(neighborPoint) || matrix(neighborPoint)) {
//						// We must be touching another block (or an edge), and therefore cannot move
//						return false;
//					}
//				}
//			}
//		}
//		return true;

		const BlockMatrix& blockMatrix = getBlockMatrix(type);
		const vec& offset = vec { modifierX, modifierY };

		for (int dy = 0; dy < 5; dy++) {
			for (int dx = 0; dx < 5; dx++) {
				const vec& currBlockOffset = vec { dx, dy };
				char currChar = blockMatrix(currBlockOffset);

				char nCurrChar = blockMatrix(currBlockOffset + rotateOffset(offset, getInverse(direction)));

				if (currChar && !nCurrChar) {
					vec neighborPoint = topRightCorner + getRotatedBlockOffset(type, { dx, dy }, direction) + offset;
					if (!matrix.contains(neighborPoint) || matrix(neighborPoint)) {
						return false;
					}
				}
			}
		}
		return true;
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	void move (Matrix<matrixWidth, matrixHeight, char>& matrix, Direction direction) {
		if (canMoveInDirection(matrix, direction)) {
			eraseFromMatrix(matrix);

			topRightCorner += vec { getDeltaX(direction), getDeltaY(direction) };

			writeToMatrix(matrix);
		}
	}
};