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
  delay(200);
  digitalWrite(powerButtonPin, LOW);
}

void SenseoControl::pressLeftButton() {
  digitalWrite(leftButtonPin, HIGH);
  delay(200);
  digitalWrite(leftButtonPin, LOW);
}

void SenseoControl::pressRightButton() {
  digitalWrite(rightButtonPin, HIGH);
  delay(200);
  digitalWrite(rightButtonPin, LOW);
}
