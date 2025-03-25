#pragma once

#include "ModularFsm/FsmComponent.h"
#include <vector>
/**
 * SenseoLedComponent is use to control the Led of Senseo
 */
class SenseoLedComponent : public FsmComponent<SenseoLedComponent>
{
public:
    SenseoLedComponent(int pLedPin);
    void update();
    void turnOn() { requestedState = RequestedState::On; }
    void turnOff() { requestedState = RequestedState::Off; }
    void blink(unsigned long milliOn, unsigned long milliOff = 0); // 0 means milliOff will be equal to milliOn

    // _burstPatern goes like that : { milliOn,milliOff,milliOn,... }
    // on the burst is complete, the led return to normal operation
    void burst(const std::vector<unsigned long> &_burstPatern);

private:
    void setLedState(bool state);

    enum class RequestedState
    {
        On,
        Off,
        Blink
    };

    std::vector<unsigned long> burstPatern;
    int burstIndex = 0;
    unsigned long timestampChanged;
    int ledPin;
    bool currentLedState = false;
    RequestedState requestedState = RequestedState::Off;
    unsigned long milliOn = 0;
    unsigned long milliOff = 0;
};
