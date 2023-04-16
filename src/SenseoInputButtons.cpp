#include "SenseoInputButtons.h"

#include "Homie.h"
#include "constants.h"

SenseoInputButtons::SenseoInputButtons(int pLedPin)
{
    analogPin = pLedPin;
    pinMode(pLedPin, INPUT);
}

void SenseoInputButtons::addButtonPushHandler(int buttonValue, const ButtonHandler &handler)
{
    handlers.push_back({.type = ON_PUSH, .buttonValue = buttonValue, .handler = handler});
    handlersByValue.insert(buttonValue);
}
void SenseoInputButtons::addButtonHoldHandler(int buttonValue, unsigned long time, const ButtonHandler &handler)
{
    handlers.push_back({.type = ON_HOLD, .buttonValue = buttonValue, .handler = handler, .time = time});
    handlersByValue.insert(buttonValue);
}

void SenseoInputButtons::addButtonReleaseHandler(int buttonValue, unsigned long time, const ButtonHandler &handler)
{
    handlers.push_back({.type = ON_RELEASE, .buttonValue = buttonValue, .handler = handler, .time = time});
    handlersByValue.insert(buttonValue);
}

bool SenseoInputButtons::isReleased(int reading) const
{
    return abs(A0NoButtonPress - reading) < A0buttonThreeshold;
}

bool SenseoInputButtons::hasHandler(int reading) const
{
    if (isReleased(reading))
        return true; // when no button is press, we don't need a handler
    
    for (const auto &handler : handlersByValue)
    {
        if (abs(handler - reading) <= A0buttonThreeshold)
            return true;
    }
    return false;
}

void SenseoInputButtons::AddToDelayedReleaseHandler(ButtonHandler handler, unsigned long timeout)
{
    // first remove obsolete element
    while (delayedReleaseHandlers.empty() == false)
    {
        auto &lastElement = delayedReleaseHandlers.back();
        if (lastElement == nullptr || millis() > lastElement->timeToDiscard)
        {
            delayedReleaseHandlers.pop_back();
        }
        else
            break;
    }

    // then add our entry
    unsigned long timeToDiscard = millis() + timeout;
    delayedReleaseHandlers.push_back(std::make_unique<DelayedHandler>(DelayedHandler{handler, timeToDiscard}));
}

SenseoInputButtons::ButtonHandler SenseoInputButtons::findBestReleaseHandler(int reading) const
{
    const HandlerData *bestHandler = nullptr;
    unsigned long releaseTime = lastReadingTime - lastReadingChangeTime;
    for (const auto &handlerData : handlers)
    {
        if (handlerData.type == ON_RELEASE && abs(handlerData.buttonValue - previousReading) <= A0buttonThreeshold)
        {
            // we press the button long enough
            if (releaseTime >= handlerData.time)
            {
                // we want to trigger only the matching handler with the longest push time
                if (bestHandler == nullptr || handlerData.time > bestHandler->time)
                {
                    bestHandler = &(handlerData);
                }
            }
        }
    }
    if (bestHandler != nullptr)
        return bestHandler->handler;
    else
        return nullptr;
}

void SenseoInputButtons::update()
{
    if (millis() - lastReadingTime < 10)
        return; // too much reading of A0 break the wifi

    int reading = analogRead(analogPin);
    unsigned long previousReadingTime = lastReadingTime;
    lastReadingTime = millis();

    if (abs(previousReading - reading) > A0buttonThreeshold)
    {
        // Manage push handlers
        // Push handlers should only be triggered if the previous reading was "released"
        // This is to avoid triggering push handler when you from A+B pressed to only A
        if (isReleased(previousReading))
        {
            for (const auto &handlerData : handlers)
            {
                if (handlerData.type == ON_PUSH && abs(handlerData.buttonValue - reading) <= A0buttonThreeshold)
                    handlerData.handler();
            }
        }

        // Manage release handlers
        if (isReleased(previousReading) == false)
        {
            ButtonHandler releaseHandler = findBestReleaseHandler(previousReading);
            if (isReleased(reading) == true)
            {
                if (releaseHandler != nullptr)
                    releaseHandler();

                // Let's trigger delayed handlers
                for (auto &delayedHandler : delayedReleaseHandlers)
                {
                    if (delayedHandler && millis() < delayedHandler->timeToDiscard)
                    {
                        delayedHandler->handler();
                        delayedHandler = nullptr;
                    }
                }
            }
            else
            {
                // When you release A+B you usually don't go straight to no reading but instead usually briefly pass by A
                // In that case we store the release handler, and if we go to no reading fast enough, we trigger the handler
                if (releaseHandler != nullptr)
                    AddToDelayedReleaseHandler(releaseHandler, 100);
            }
        }

        lastReadingChangeTime = lastReadingTime; // need to stay at the end of the "if"
    }
    else
    {
        // Manage Hold handlers
        for (const auto &handlerData : handlers)
        {
            if (handlerData.type == ON_HOLD && abs(handlerData.buttonValue - previousReading) <= A0buttonThreeshold)
            {
                unsigned long triggerTime = lastReadingChangeTime + handlerData.time;
                if (previousReadingTime < triggerTime && lastReadingTime >= triggerTime)
                    handlerData.handler();
            }
        }
    }

    if (!hasHandler(reading))
        Homie.getLogger() << "Button Handler not found for " << reading << endl;
    previousReading = reading;
}
