/*
  SenseoLed.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef SenseoLed_h
#define SenseoLed_h

#include "Arduino.h"
#include "config.h"

enum ledStateEnum {
  LED_unknown,
  LED_OFF,
  LED_SLOW,
  LED_FAST,
  LED_ON
};

class SenseoLed
{
  public:
    SenseoLed(int ledPin);
    void setLedChangedAt(unsigned long ms);
    int getLastPulseDuration();
    void updateState();
    bool hasChanged();
    ledStateEnum getState();
    String getStateAsString();
  private:
    int ocSenseLedPin;
    bool ledChanged = false;
    unsigned long ledChangeMillis = 0;
    unsigned long prevLedChangeMillis = 0;
    ledStateEnum ledState = LED_unknown;
    ledStateEnum ledStatePrev = LED_unknown;
};

#endif
