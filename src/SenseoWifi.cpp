/*
SenseoWifi.cpp - base file for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include <Homie.h>

#include "SenseoLed.h"
#include "SenseoSM.h"
#include "SenseoControl.h"
#include "Cup.h"
#include "pins.h"
#include "constants.h"
#include "testIO.cpp"

SenseoLed mySenseoLed(ocSenseLedPin);
SenseoSM mySenseoSM;
SenseoControl myControl(ocPressPowerPin, ocPressLeftPin, ocPressRightPin);
Cup myCup(cupDetectorPin);

HomieNode senseoNode("machine", "machine");
HomieSetting<bool> CupDetectorAvailableSetting("available", "Enable cup detection (TCRT5000)");
HomieSetting<bool> DebuggingSetting("debugging", "Enable debugging output over MQTT");
HomieSetting<bool> BuzzerSetting("buzzer", "Enable buzzer feedback (no water, cup finished, ...)");

unsigned long lastResendTime = 0;

// will get called by the LED changed interrupt
void ledChangedHandler() {
  mySenseoLed.pinStateToggled();
  Homie.getLogger() << "LED pulse duration: " << mySenseoLed.getLastPulseDuration() << endl;
}

//
bool powerHandler(const HomieRange& range, const String& value) {
  Homie.getLogger() << "MQTT topic '/power' message: " << value << endl;
  if (value != "ON" || value !="OFF") {
    Homie.getLogger() << "--> malformed message content. Allowed: [ON,OFF]" << endl;
    return false;
  }
  
  if (value == "ON" && mySenseoSM.getState() == SENSEO_OFF) {
    myControl.pressPowerButton();
  }
  else if (value == "OFF" && mySenseoSM.getState() != SENSEO_OFF) {
    myControl.pressPowerButton();
  }
  else {
    // nothing to do here, machine already in right state
    senseoNode.setProperty("power").send(value);
  }
  return true;
}

//
bool brewHandler(const HomieRange& range, const String& value) {
  Homie.getLogger() << "MQTT topic '/brew' message: " << value << endl;
  if (value != "1cup" || value !="2cup") {
    Homie.getLogger() << "--> malformed message content. Allowed: [1cup,2cup]" << endl;
    senseoNode.setProperty("brew").send("false");
    return false;
  }
  
  if (mySenseoSM.getState() != SENSEO_READY) {
    Homie.getLogger() << "--> wrong state" << endl;
    senseoNode.setProperty("brew").send("false");
    return false;
  }
  
  if (CupDetectorAvailableSetting.get()) {
    if (myCup.isNotAvailable() || myCup.isFull()) {
      Homie.getLogger() << "--> no or full cup detected" << endl;
      senseoNode.setProperty("brew").send("false");
      return false;
    }
  }
  
  if (value == "1cup") myControl.pressLeftButton();
  if (value == "2cup") myControl.pressRightButton();
  return true;
}

//
void senseoStateEntryAction() {
  switch (mySenseoSM.getState()) {
    case SENSEO_OFF: {
      senseoNode.setProperty("power").send("OFF");
      break;
    }
    case SENSEO_HEATING: {
      break;
    }
    case SENSEO_READY: {
      if (BuzzerSetting.get()) tone(beeperPin, 1024, 500);
      break;
    }
    case SENSEO_BREWING: {
      senseoNode.setProperty("brew").send("true");
      break;
    }
    case SENSEO_NOWATER: {
      if (BuzzerSetting.get()) tone(beeperPin, 2048, 1000);
      senseoNode.setProperty("outOfWater").send("true");
      break;
    }
  }
}

//
void senseoStateExitAction() {
  switch (mySenseoSM.getStatePrev()) {
    case SENSEO_OFF: {
      senseoNode.setProperty("power").send("ON");
      break;
    }
    case SENSEO_HEATING: {
      break;
    }
    case SENSEO_READY: {
      break;
    }
    case SENSEO_BREWING: {
      if (CupDetectorAvailableSetting.get()) myCup.fillUp();
      senseoNode.setProperty("brew").send("false");
      senseoNode.setProperty("cupFull").send("true");
      // 0---------------------|-----+-----|-----+-----|-------100
      int tolerance = (BrewingTime2Cup - BrewingTime1Cup) / 2;
      if (abs(mySenseoSM.getSecondsInLastState() - BrewingTime1Cup) < tolerance) {
        senseoNode.setProperty("brewedSize").send("1");
      }
      else if (abs(mySenseoSM.getSecondsInLastState() - BrewingTime2Cup) < tolerance) {
        senseoNode.setProperty("brewedSize").send("2");
      }
      else {
        senseoNode.setProperty("brewedSize").send("0");
        senseoNode.setProperty("debug").send("unexpected time in SENSEO_BREWING state.");
        senseoNode.setProperty("debug").send(String(mySenseoSM.getSecondsInLastState()));
      }
      break;
    }
    case SENSEO_NOWATER: {
      senseoNode.setProperty("outOfWater").send("false");
      break;
    }
  }
}

//
void setupHandler() {
  attachInterrupt(digitalPinToInterrupt(ocSenseLedPin), ledChangedHandler, CHANGE);
}

//
void loopHandler() {
  if (millis() - lastResendTime >= resendInterval * 1000UL || lastResendTime == 0) {
    Homie.getLogger() << "Senseo state: " << mySenseoSM.getStateAsString() << endl;
    senseoNode.setProperty("opState").send(mySenseoSM.getStateAsString());
    lastResendTime = millis();
  }
  
  if (CupDetectorAvailableSetting.get()) {
    myCup.updateState();
    if (myCup.hasChanged()) {
      Homie.getLogger() << "Cup state changed. Available: " << (myCup.isAvailable() ? "yes" : "no") << endl;
      senseoNode.setProperty("cupAvailable").send(myCup.isAvailable() ? "true" : "false");
      senseoNode.setProperty("cupFull").send(myCup.isFull() ? "true" : "false");
    }
  }
  
  mySenseoLed.updateState();
  if (mySenseoLed.hasChanged()) {
    Homie.getLogger() << "LED state machine, new LED state: " << mySenseoLed.getStateAsString() << endl;
    senseoNode.setProperty("ledState").send(mySenseoLed.getStateAsString());
  }
  
  mySenseoSM.updateState(mySenseoLed.getState());
  if (mySenseoSM.stateHasChanged()) {
    Homie.getLogger() << "(time in last state: " << mySenseoSM.getSecondsInLastState() << "s)" << endl;
    Homie.getLogger() << "Senseo state machine, new Senseo state: " << mySenseoSM.getStateAsString() << endl;
    senseoNode.setProperty("opState").send(mySenseoSM.getStateAsString());
    
    senseoStateEntryAction();
    senseoStateExitAction();
  }
}

//
void setup() {
  Serial.begin(115200);
  Serial << endl << endl << "☕☕☕☕ Enjoy your SenseoWifi ☕☕☕☕" << endl << endl;
  
  pinMode(ocPressLeftPin, OUTPUT);
  pinMode(ocPressRightPin, OUTPUT);
  pinMode(ocPressPowerPin, OUTPUT);
  pinMode(ocSenseLedPin, INPUT_PULLUP);
  
  digitalWrite(ocPressPowerPin, LOW);
  digitalWrite(ocPressLeftPin, LOW);
  digitalWrite(ocPressRightPin, LOW);
  
  pinMode(beeperPin, OUTPUT);
  pinMode(resetButtonPin, OUTPUT);
  
  if (CupDetectorAvailableSetting.get()) {
    pinMode(cupDetectorPin, INPUT_PULLUP);
    pinMode(cupDetectorAnalogPin, INPUT);
    myCup.initDebouncer();
  }
  
  Homie_setFirmware("senseo-wifi-wemos", "0.9.0");
  Homie_setBrand("SenseoWifi");
  Homie.setResetTrigger(resetButtonPin, LOW, 5000);
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);
  
  CupDetectorAvailableSetting.setDefaultValue(false);
  BuzzerSetting.setDefaultValue(true);
  DebuggingSetting.setDefaultValue(false);
  
  senseoNode.advertise("ledState");
  senseoNode.advertise("opState");
  senseoNode.advertise("power").settable(powerHandler);
  senseoNode.advertise("brew").settable(brewHandler);
  if (CupDetectorAvailableSetting.get()) senseoNode.advertise("cupAvailable");
  senseoNode.advertise("cupFull");
  senseoNode.advertise("brewedSize");
  senseoNode.advertise("outOfWater");
  senseoNode.advertise("debug");
  
  // test the circuit and Senseo connections, will loop indefinitely
  //testIO();
  
  Homie.setup();
}

//
void loop() {
  Homie.loop();
}
