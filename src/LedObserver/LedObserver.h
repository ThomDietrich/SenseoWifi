/*
  LedObserver.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef SenseoLed_h
#define SenseoLed_h

#include "ILedObserver.h"

class HomieNode;

class LedObserver : public ILedObserver
{
public:
    LedObserver(HomieNode &senseoNode, int ledPin);

    void attachInterrupt() override;
    void detachInterrupt() override;
    void onMqttReady() override;

    void updateState() override;    

    ledStateEnum getState() const override;

private:
    static void ledChangedIsr();

    int ledPin;
    unsigned long lastUpdateMillis = 0;
    int lastPulseDuration = 0;
    ledStateEnum ledState = LED_unknown;    
    HomieNode &senseoNode;
};

#endif
