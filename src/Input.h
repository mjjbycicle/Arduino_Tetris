#pragma once

#include "Constants.h"

namespace input {
	constexpr uint8_t JOYSTICK_BUTTON = 12;
	constexpr uint8_t JOYSTICK_X_IN = A3;
	constexpr uint8_t JOYSTICK_Y_IN = A4;

	constexpr int JOYSTICK_TOLERANCE = 338;

    void setup() {
        pinMode(JOYSTICK_BUTTON, INPUT);
    }

    bool isJoystickLeft() {
        int x = analogRead(JOYSTICK_X_IN);
        return x <= 0x200 - JOYSTICK_TOLERANCE;
    }

    bool isJoystickRight() {
        int x = analogRead(JOYSTICK_X_IN);
        return x >= 0x200 + JOYSTICK_TOLERANCE;
    }

    bool isJoystickDown() {
        int y = analogRead(JOYSTICK_Y_IN);
        return y >= 0x200 + JOYSTICK_TOLERANCE;
    }

    bool isJoystickUp() {
        int y = analogRead(JOYSTICK_Y_IN);
        return y <= 0x200 - JOYSTICK_TOLERANCE;
    }

    bool isJoystickClicked() {
        return digitalRead(JOYSTICK_BUTTON) == HIGH;
    }
}