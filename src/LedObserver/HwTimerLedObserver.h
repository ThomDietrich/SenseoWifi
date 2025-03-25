#pragma once

#include "ILedObserver.h"
class HomieNode;

// LedObserver based on the hardware timer.
// More accurate than the "regular" Led observer and not impacted by the time spent in the main loop as this is 100% interrupt based
// Unfortunately the esp8266 has only two hardware timer, timer0 is used for the wifi and timer1 that I used here is used as well by the tone() function
// So basicaly, it's either the buzzer or the timer :-/

class HwTimerLedObserver : public ILedObserver
{
public:
    HwTimerLedObserver(HomieNode &senseoNode, int pin);
    virtual void attachInterrupt() override;
    virtual void detachInterrupt() override;
    virtual ledStateEnum getState() const override;
    virtual void updateState() override;

private:
    static void timerElapseIsr();
    static void ledChangedIsr();

    int ledPin;
    HomieNode &senseoNode;
    ledStateEnum prevState = LED_unknown;
};