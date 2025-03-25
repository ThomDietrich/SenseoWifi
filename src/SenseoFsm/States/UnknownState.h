#pragma once

#include "SenseoState.h"

class ILedObserver;

class UnknownState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_UNKNOWN");
        UnknownState(const ILedObserver & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onUpdate() override;
        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
};
