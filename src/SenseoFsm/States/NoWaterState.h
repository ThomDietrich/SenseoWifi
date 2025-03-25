#pragma once
#include "SenseoState.h"

class ILedObserver;
class HomieNode;

class NoWaterState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_NOWATTER");
        NoWaterState(const ILedObserver & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;
    private:
        //const LedObserver & senseoLed;
        //HomieNode & senseoNode;
};
