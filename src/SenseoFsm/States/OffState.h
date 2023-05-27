#pragma once
#include "SenseoState.h"

class ILedObserver;
class HomieNode;
class ProgramComponent;

class OffState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_OFF");
        OffState(const ILedObserver & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onInitialized() override;
        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;

    private: 
        ProgramComponent * programComponent;
    
};
