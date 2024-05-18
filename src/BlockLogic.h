#include "Blocks.h"
#include "Coordinates.h"
#include "Helpers.h"

#ifndef TETRIS_BLOCKLOGIC_H
#define TETRIS_BLOCKLOGIC_H

bool occupiedInMatrix(char matrix[24][10], int r, int c);

bool occupiedInBlock(char matrix[5][5], int r, int c);

void rotateNTimes(char (&dest)[5][5], char block[5][5], char type, Mod4 numRot);

void rotate(char (&dest)[5][5], char block[5][5], char type);

class Block {
private:
    Coordinate topRightCorner;
    bool set;
    char type;
    Mod4 numRot;

public:
    Block(int x, int y, char type, int numRot) {
        this->topRightCorner.set(x, y);
        this->set = false;
        this->type = type;
        this->numRot.set(numRot);
    }

    void getDisplayVec(char (&arr)[5][5]) {
        char displayArr[5][5];
        getRawVec(arr, type);
    }

    char getType() {
        return type;
    }

    void writeToMatrix(char (&matrix)[24][10]) {
        char currArr[5][5];
        getDisplayVec(currArr);
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                char currChar = currArr[r][c];
                if (currChar !=  0 ) {
                    matrix[r + topRightCorner.getY()][c + topRightCorner.getX()] = currChar;
                }
            }
        }
    }

    void eraseFromMatrix(char (&matrix)[24][10]) {
        char currArr[5][5];
        getDisplayVec(currArr);
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                char currChar = currArr[r][c];
                if (currChar !=  0 ) {
                    matrix[r + topRightCorner.getY()][c + topRightCorner.getX()] =  0 ;
                }
            }
        }
    }

    void moveDown(char (&matrix)[24][10]) {
        if (canMoveDown(matrix)) {
            eraseFromMatrix(matrix);
            topRightCorner.moveDown();
            writeToMatrix(matrix);
        } else {
            set = true;
        }
    }

    void moveLeft(char (&matrix)[24][10]) {
        if (canMoveSides(matrix, true)) {
            eraseFromMatrix(matrix);
            topRightCorner.moveLeft();
            writeToMatrix(matrix);
        }
    }

    void moveRight(char (&matrix)[24][10]) {
        if (canMoveSides(matrix, false)) {
            eraseFromMatrix(matrix);
            topRightCorner.moveRight();
            writeToMatrix(matrix);
        }
    }

    bool canMoveDown(char matrix[24][10]) {
        char currArr[5][5];
        getDisplayVec(currArr);
        for (int i = 1; i <= 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (currArr[i - 1][j] !=  0
                    and occupiedInMatrix(matrix, topRightCorner.getY() + i, topRightCorner.getX() + j)) {
                    if (i != 5) {
                        if (currArr[i][j] ==  0 ) {
                            return false;
                        }
                    } else
                        return false;
                }
            }
        }
        return true;
    }

    bool canMoveSides(char matrix[24][10], bool movingLeft) {
        char currArr[5][5];
        getDisplayVec(currArr);
        int modifier = movingLeft ? -1 : 1;
        for (int i = 0; i < 5; i++) {
            for (int j = modifier; 5 + modifier; j++) {
                if (currArr[i][j - modifier] !=  0
                    and occupiedInMatrix(matrix, topRightCorner.getY() + i, topRightCorner.getX() + j)) {
                    if (j != -1 and j != 5) {
                        if (currArr[i][j] ==  0 )
                            return false;
                    } else
                        return false;
                }
            }
        }
        return true;
    }

    bool isSet() {
        return set;
    }
};


void rotate3X3(char (&dest)[5][5], char block[5][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            dest[i][j] = block[5 - j - 1][i];
        }
    }
}

void rotateOther(char (&dest)[5][5], char block[5][5]) {
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            if (c == 0 or r == 4) {
                dest[r][c] =  0 ;
            } else {
                dest[c - 1][5 - r - 1] = block[r][c];
            }
        }
    }
}

void rotate(char (&dest)[5][5], char block[5][5], char type) {
    if (type == 'Q' or type == 'I') {
        rotateOther(dest, block);
    } else {
        rotate3X3(dest, block);
    }
}

void rotateNTimes(char (&dest)[5][5], char block[5][5], char type, Mod4 numRot) {

}

bool occupiedInMatrix(char matrix[24][10], int r, int c) {
    if (r == 24) return true;
    if (c == -1) return true;
    if (c == 10) return true;
    if (matrix[r][c] !=  0 ) return true;
    return false;
}

bool occupiedInBlock(char matrix[5][5], int r, int c) {
    if (matrix[r][c] !=  0 ) return true;
    return false;
}

#endif //TETRIS_BLOCK_LOGIC_H