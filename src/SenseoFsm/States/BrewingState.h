#pragma once
#include "SenseoState.h"

class LedObserver;
class HomieNode;

class BrewingState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_BREWING");

        BrewingState(const ILedObserver & led, HomieNode & node) : SenseoState(led,node) {}

        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;
    
    private:
        bool brewingCancel = false;
};
