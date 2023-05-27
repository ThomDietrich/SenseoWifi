#include "SenseoLedComponent.h"
#include "Arduino.h"

SenseoLedComponent::SenseoLedComponent(int pLedPin)
{
    ledPin = pLedPin;
    pinMode(ledPin, OUTPUT);
    setLedState(false);
}

void SenseoLedComponent::setLedState(bool state)
{
    timestampChanged = millis();
    currentLedState = state;
    digitalWrite(ledPin, state ? HIGH : LOW);
}

void SenseoLedComponent::blink(unsigned long _milliOn, unsigned long _milliOff /*=0*/)
{
    milliOn = _milliOn;
    milliOff = _milliOff > 0 ? _milliOff : _milliOn;
    requestedState = RequestedState::Blink;
}

void SenseoLedComponent::burst(const std::vector<unsigned long> &_burstData)
{
    burstPatern = _burstData;
    burstIndex = 0;
}

void SenseoLedComponent::update()
{
    unsigned long currentMillis = millis();
    if (burstPatern.empty() == false)
    {
        // the patern goes like this : { milliOn, milliOff, milliOn, etc... }
        // so even burstIndex are milliOn and odd burstIndex are milliOff
        if ((burstIndex % 2) == 0)
        {
            if (currentLedState == false)
                setLedState(true);
            else if (currentMillis - timestampChanged >= burstPatern[burstIndex])
            {
                setLedState(false);
                burstIndex++;
            }
        }
        else
        {
            if (currentLedState == true)
                setLedState(false);
            else if (currentMillis - timestampChanged >= burstPatern[burstIndex])
            {
                setLedState(true);
                burstIndex++;
            }
        }
        if ((size_t)burstIndex >= burstPatern.size())
            burstPatern.clear();
    }
    else
    {
        switch (requestedState)
        {
        case RequestedState::Off:
            setLedState(false);
            break;
        case RequestedState::On:
            setLedState(true);
            break;
        case RequestedState::Blink:
            if (currentLedState == true && currentMillis - timestampChanged >= milliOn)
            {
                setLedState(false);
            }
            else if (currentLedState == false && currentMillis - timestampChanged >= milliOff)
            {
                setLedState(true);
            }
            break;
        }
    }
}
