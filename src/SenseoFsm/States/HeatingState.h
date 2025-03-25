#pragma once
#include "SenseoState.h"

class LedObserver;

class HeatingState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_HEATING");
        HeatingState(const ILedObserver & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onEnter(StateId previousState) override;
        //virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;
    private:
        //const LedObserver & senseoLed;
};
