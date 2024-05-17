#include "Constants.h"

#ifndef TETRIS_INPUT_H
#define TETRIS_INPUT_H

namespace input {
    void setup() {
        pinMode(JOYSTICK_BUTTON, INPUT);
    }

    bool isJoystickLeft() {
        int x = analogRead(X_IN);
        return x <= 174;
    }

    bool isJoystickRight() {
        int x = analogRead(X_IN);
        return x >= 850;
    }

    bool isJoystickDown() {
        int y = analogRead(Y_IN);
        return y >= 850;
    }

    bool isJoystickUp() {
        int y = analogRead(Y_IN);
        return y <= 174;
    }

    bool isClicked() {
        return digitalRead(JOYSTICK_BUTTON) == HIGH;
    }
}

#endif //TETRIS_INPUT_H