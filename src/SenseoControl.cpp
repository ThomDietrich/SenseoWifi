/*
SenseoControl.cpp - Library for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include "SenseoControl.h"

SenseoControl::SenseoControl(int pBPin, int lBPin,  int rBPin) {
  powerButtonPin = pBPin;
  leftButtonPin = lBPin;
  rightButtonPin = rBPin;
}

void SenseoControl::pressPowerButton() {
  digitalWrite(powerButtonPin, HIGH);
  timestampPressed = millis();
}

void SenseoControl::pressLeftButton() {
  digitalWrite(leftButtonPin, HIGH);
  timestampPressed = millis();
}

void SenseoControl::pressRightButton() {
  digitalWrite(rightButtonPin, HIGH);
  timestampPressed = millis();
}

/**
 * The 'press..Button()' functions will "press" but not release the buttons.
 * Call this method regularly (every loop() run) to release buttons after some time.
 */
void SenseoControl::releaseIfPressed() {
  if ((timestampPressed != 0) && (millis() - timestampPressed >= pressDurationMillis)) {
    digitalWrite(powerButtonPin, LOW);
    digitalWrite(leftButtonPin, LOW);
    digitalWrite(rightButtonPin, LOW);
    timestampPressed = 0;
  }
}
