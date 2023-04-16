/*
ControlComponent.cpp - Library for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include "ControlComponent.h"
#include <Arduino.h>
#include "constants.h"

ControlComponent::ControlComponent(int pBPin, int lBPin, int rBPin)
{
    powerButtonPin = pBPin;
    leftButtonPin = lBPin;
    rightButtonPin = rBPin;

    pinMode(leftButtonPin, OUTPUT);
    pinMode(rightButtonPin, OUTPUT);
    pinMode(powerButtonPin, OUTPUT);

    digitalWrite(powerButtonPin, LOW);
    digitalWrite(leftButtonPin, LOW);
    digitalWrite(rightButtonPin, LOW);
}

void ControlComponent::pressPowerButton()
{
    digitalWrite(powerButtonPin, HIGH);
    timestampPressed = millis();
}

void ControlComponent::pressLeftButton()
{
    digitalWrite(leftButtonPin, HIGH);
    timestampPressed = millis();
}

void ControlComponent::pressRightButton()
{
    digitalWrite(rightButtonPin, HIGH);
    timestampPressed = millis();
}

void ControlComponent::pressLeftRightButton()
{
    digitalWrite(leftButtonPin, HIGH);
    digitalWrite(rightButtonPin, HIGH);
    timestampPressed = millis();
}

/**
 * The 'press..Button()' functions will "press" but not release the buttons.
 * Call this method regularly (every loop() run) to release buttons after some time.
 */
void ControlComponent::update()
{
    if ((timestampPressed != 0) && (millis() - timestampPressed >= pressDuration))
    {
        digitalWrite(powerButtonPin, LOW);
        digitalWrite(leftButtonPin, LOW);
        digitalWrite(rightButtonPin, LOW);
        timestampPressed = 0;
    }
}
