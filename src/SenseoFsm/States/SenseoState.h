#pragma once
#include "ModularFsm/FsmState.h"

#include "SenseoFsm/Components/CommandComponent.h"

class ILedObserver;
class HomieNode;
class ControlComponent;
class CupComponent;

class SenseoState : public FsmState
{    
    public:
        SenseoState(const ILedObserver & led, HomieNode & node) : senseoLed(led),senseoNode(node) {}

        void onInitialized() override;

        bool hasOffCommands() const;
        void processOffCommands();
        bool hasPendingCommands(CommandComponent::CommandBitFields commands) const { return commandComponent->hasPendingCommands(commands); }
        bool hasProcessedCommands(CommandComponent::CommandBitFields commands) const { return commandComponent->hasProcessedCommands(commands); }
        void processCommands(CommandComponent::CommandBitFields commands);
        void clearCommands(CommandComponent::CommandBitFields commands);

    protected:
        const ILedObserver & senseoLed;
        HomieNode & senseoNode;
        CommandComponent * commandComponent = nullptr;    
        ControlComponent * controlComponent = nullptr; 
        CupComponent * cupComponent = nullptr;   
};