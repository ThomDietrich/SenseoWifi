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

void SenseoControl::setMqttMessage(String msg) {
  mqttMsg = msg;
}


bool SenseoControl::hasMqttMsg() {
  return (mqttMsg != "");
}


bool SenseoControl::reactOnMqttMsg(senseoStateEnum senseoState) {
  Serial.print("Nachricht per MQTT erhalten: ");
  Serial.println(mqttMsg);
  if (mqttMsg == "ON") {
    if (senseoState == SENSEO_OFF) {
      pressPowerButton();
    }
  } else if (mqttMsg == "OFF") {
    if (senseoState != SENSEO_OFF) {
      pressPowerButton();
    }
  } else if (mqttMsg == "1-cup") {
    if (senseoState == SENSEO_READY) {
      pressLeftButton();
    }
  } else if (mqttMsg == "2-cup") {
    if (senseoState == SENSEO_READY) {
      pressRightButton();
    }
  } else {
    mqttMsg = "";
    return false;
  }
  mqttMsg = "";
  return true;
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

