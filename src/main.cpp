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
#include <AlmostRandom.h>
AlmostRandom ra;

Display<10, 24> display(pin{11}, pin{9}, pin{10}, pin{A0}, pin{A1}, pin{A2});

char getRandomBlock();

Block currBlock(0, 3, getRandomBlock(), 0);
Cooldown dropCooldown(4);
Cooldown lockCooldown(5);

char getRandomBlock() {
	int r = (ra.getRandomInt() % 7 + 7) % 7;
	return "QSTRILZ"[r];
}

void setup() {
    display.setup();
	randomSeed(analogRead(A7));
}

void loop() {
	if(input::isJoystickUp()) {
		if(input::isJoystickLeft()) {
			// TODO
		}
		else if(input::isJoystickRight()) {
			// TODO
		}
	}
	if(input::isJoystickLeft() && currBlock.canMoveInDirection(display.matrix(), DIRECTION_L)) {
		currBlock.move(display.matrix(), DIRECTION_L);
		lockCooldown.reset();
	}
	if(input::isJoystickRight() && currBlock.canMoveInDirection(display.matrix(), DIRECTION_R)) {
		currBlock.move(display.matrix(), DIRECTION_R);
		lockCooldown.reset();
	}
	if(input::isJoystickDown()) {
		currBlock.move(display.matrix(), DIRECTION_D);
		dropCooldown.reset();
	}
	else if(dropCooldown.isFinished()) {
		currBlock.move(display.matrix(), DIRECTION_D);
		dropCooldown.reset();
	}

	if(!currBlock.canMoveInDirection(display.matrix(), DIRECTION_D) && lockCooldown.isFinished()) {
		currBlock = Block(0, 0, getRandomBlock(), 0);
		dropCooldown.skip();
	}

	dropCooldown.tick();
	lockCooldown.tick();

	display.update();
	delay(100);
}
