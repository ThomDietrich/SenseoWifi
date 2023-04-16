#pragma once

enum ledStateEnum : char // this garanty that accessing the enum is atomic (ie one instruction)
{
    LED_unknown,
    LED_OFF,
    LED_SLOW,
    LED_FAST,
    LED_ON
};

class ILedObserver
{
public:
    virtual ledStateEnum getState() const = 0;
    virtual const char * getStateAsString() final
    {
        ledStateEnum ledState = getState();
        if (ledState == LED_OFF) return "LED_OFF";
        else if (ledState == LED_SLOW) return "LED_SLOW";
        else if (ledState == LED_FAST) return "LED_FAST";
        else if (ledState == LED_ON) return "LED_ON";
        else return "LED_unknown";
    }

    virtual void attachInterrupt() {}
    virtual void detachInterrupt() {}
    virtual void updateState() {}
    virtual void onMqttReady() {}
};