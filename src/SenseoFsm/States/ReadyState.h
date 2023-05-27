#pragma once
#include "SenseoState.h"

class ILedObserver;

class ReadyState : public SenseoState
{    
    public:
        DECLARE_STATE("SENSEO_READY");
        
        ReadyState(const ILedObserver & led, HomieNode & node, bool _useCustomizableButtonAddon) : SenseoState(led,node),useCustomizableButtonAddon(_useCustomizableButtonAddon) {}
        virtual void onEnter(StateId previousState) override;
        virtual void onUpdate() override;
    protected:
        void processBrewingCommand(CommandComponent::Command command);
        bool waitingForACup = false;
        unsigned long lastLedBurst = 0;
        bool useCustomizableButtonAddon;
};
