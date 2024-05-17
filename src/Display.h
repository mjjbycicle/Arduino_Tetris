#include <RGBmatrixPanel.h>
#include "Coordinates.h"
#include "BlockLogic.h"
#include "Blocks.h"
#include "Helpers.h"

#ifndef TETRIS_DISPLAY_H
#define TETRIS_DISPLAY_H

void writePixelToPanel(char pixel, RGBmatrixPanel &panel, Coordinate matrixCoord);

void displayToPanel(char matrix[24][10], RGBmatrixPanel &panel) {
    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 10; x++) {
            Coordinate currCoord;
            currCoord.set(x, y);
            writePixelToPanel(matrix[y][x], panel, currCoord);
        }
    }
}

void writePixelToPanel(char pixel, RGBmatrixPanel &panel, Coordinate matrixCoord) {
    uint16_t color = getColor(pixel);
    panel.drawPixel(
            matrixCoord.getBoardX(),
            matrixCoord.getBoardY(),
            color
    );
}

#endif //TETRIS_DISPLAY_H