#include "Display.h"
#include "Game.h"
#include "Counter.h"
#include "Coordinates.h"
#include "Constants.h"
#include "BlockLogic.h"
#include "Blocks.h"
#include "Input.h"
#include "Helpers.h"
#include "Arduino.h"
#define CLK  11
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2


//RGBmatrixPanel panel(A, B, C, CLK, LAT, OE, false);

//char matrix[24][10]

Display<10, 24> display(pin{CLK}, pin{OE}, pin{LAT}, pin{A}, pin{B}, pin{C});

void setup() {
    display.setup();

//    Block b(0, 0, 'L', 0);
//    b.writeToMatrix(display.matrix());
//    Block c(5, 12, 'T', 0);
//    c.writeToMatrix(display.matrix());

	display.matrix()[0][0] = 'L';
	display.matrix()[23][9] = 'R';

    display.update();
}

void loop() {
}