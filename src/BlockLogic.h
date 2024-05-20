#pragma once

#include "sprites.h"
#include "Vec.h"
#include "matrix.h"
#include "util.h"

vec getRotatedBlockOffset (char type, vec p, Rotation direction) {
	if (type == 'Q') return p;
	else if (type == 'I') {
		return direction.get() & 1 ?
			   vec { p.y() + 0, 4 - p.x() } :
			   p;
	}
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

vec rotateOffset (vec offset, Rotation direction) {
	switch (direction.get()) {
		case ROTATE_NONE.get():
			return offset;
		case ROTATE_90_CCW.get():
			return { +offset.y(), -offset.x() };
		case ROTATE_180.get():
			return { -offset.x(), -offset.y() };
		case ROTATE_90_CW.get():
			return { -offset.y(), +offset.x() };
		default:
			exit(1);
	}
}

class Block {
	char type;

public:
	Rotation rotation;
	vec topRightCorner;

	Block (int8_t x, int8_t y, char type, Rotation rotation) : type(type), rotation(rotation), topRightCorner(x, y) {}

	char getType() {
		return type;
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	void writeToMatrix (Matrix<matrixWidth, matrixHeight, char>& matrix) const {
		const BlockMatrix& blockMatrix = getBlockMatrix(type);
		for (int8_t dy = 0; dy < 5; dy++) {
			for (int8_t dx = 0; dx < 5; dx++) {
				char currChar = blockMatrix(dx, dy);
				if (currChar) {
					matrix(topRightCorner + getRotatedBlockOffset(type, { dx, dy }, rotation)) = currChar;
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
					matrix(topRightCorner + getRotatedBlockOffset(type, { dx, dy }, rotation)) = 0;
				}
			}
		}
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	bool tryApplyModification (Matrix<matrixWidth, matrixHeight, char>& matrix, vec offset, Rotation newRotation) {
		eraseFromMatrix(matrix);

		vec oldCorner = topRightCorner;
		Rotation oldRotation = rotation;

		topRightCorner += offset;
		rotation = newRotation;

		const BlockMatrix& blockMatrix = getBlockMatrix(type);
		for (int dy = 0; dy < 5; dy++) {
			for (int dx = 0; dx < 5; dx++) {
				if (blockMatrix({ dx, dy })) {
					const vec& pos = topRightCorner + getRotatedBlockOffset(type, { dx, dy }, rotation);
					if (!matrix.contains(pos) || matrix(pos) != 0) {
						goto modificationFailed;
					}
				}
			}
		}

		writeToMatrix(matrix);
		return true;

		modificationFailed:
		topRightCorner = oldCorner;
		rotation = oldRotation;
		writeToMatrix(matrix);
		return false;
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	bool tryRotate (Matrix<matrixWidth, matrixHeight, char>& matrix, Rotation direction) {
		return tryApplyModification(matrix, {}, rotation + direction);
	}

	template <int8_t matrixWidth, int8_t matrixHeight>
	bool tryMove (Matrix<matrixWidth, matrixHeight, char>& matrix, Rotation direction) {
		return tryApplyModification(matrix, getOffset(direction), rotation);
	}
};