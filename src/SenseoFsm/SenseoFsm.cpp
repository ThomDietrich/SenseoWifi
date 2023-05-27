#include "SenseoFsm.h"

#include "ModularFsm/ModularFsm.h"
#include "Components/SenseoLedComponent.h"
#include "Components/BuzzerComponent.h"
#include "Components/CupComponent.h"
#include "Components/ControlComponent.h"
#include "Components/CommandComponent.h"
#include "Components/ProgramComponent.h"
#include "States/BrewingState.h"
#include "States/HeatingState.h"
#include "States/NoWaterState.h"
#include "States/OffState.h"
#include "States/ReadyState.h"
#include "States/UnknownState.h"
#include "pins.h"
#include <Homie.h>


void SenseoFsm::setup(const ILedObserver & led,bool useCupDetector, bool useBuzzer, bool useCustomizableButton) 
{
    useCustomizableButtonAddon = useCustomizableButton;
    addComponent(std::make_unique<CommandComponent>(senseoNode));
    addComponent(std::make_unique<ProgramComponent>(senseoNode));
    addComponent(std::make_unique<ControlComponent>(ocPressPowerPin, ocPressLeftPin, ocPressRightPin));
    
    if (useBuzzer) addComponent(std::make_unique<BuzzerComponent>(beeperPin));
    if (useCupDetector) addComponent(std::make_unique<CupComponent>(senseoNode,cupDetectorPin));
    if (useCustomizableButton) addComponent(std::make_unique<SenseoLedComponent>(senseoLedOutPin));

    addState(std::make_unique<BrewingState>(led,senseoNode));
    addState(std::make_unique<HeatingState>(led,senseoNode));
    addState(std::make_unique<NoWaterState>(led,senseoNode));
    addState(std::make_unique<OffState>(led,senseoNode));
    addState(std::make_unique<ReadyState>(led,senseoNode,useCustomizableButtonAddon));
    addState(std::make_unique<UnknownState>(led,senseoNode));

    setInitialState<UnknownState>();
}

bool SenseoFsm::sendCommands(CommandComponent::CommandBitFields commands)
{
    CommandComponent * commandComponent = getComponent<CommandComponent>();
    if (commandComponent != nullptr && currentState != nullptr)
    {
        if ((commandComponent->hasCommand(commands,CommandComponent::Brew1Cup) || commandComponent->hasCommand(commands,CommandComponent::Brew2Cup)) && isInState<BrewingState>())
        {
          Homie.getLogger() << "Commands " << commandComponent->getCommandsAsString(commands) << " rejected because we are already brewing" << endl;
          return false;
        }

        Homie.getLogger() << currentState->getStateName() << ": Sending commands " << commandComponent->getCommandsAsString(commands) << endl;
        commandComponent->sendCommands(commands);
        return true;
    }
    return false;
}

bool SenseoFsm::isOff() 
{
    return isInState<OffState>() || isInState<UnknownState>();
}

void SenseoFsm::onStateChange(FsmState * prevState, FsmState * nextState) 
{
  if (true) Homie.getLogger() << "Senseo state machine, new Senseo state: " << nextState->getStateName() << endl;
  
  senseoNode.setProperty("opState").send(nextState->getStateName());
  if (prevState != nullptr) 
  {
    int secondInPrevState = (prevState->getTimeInState() + 500) / 1000;
    senseoNode.setProperty("debug").send(
      String("senseoState: Switching from ") + String(prevState->getStateName())
      + String(" to ") + String(nextState->getStateName())
      + String(" after ") + String(secondInPrevState) + String(" seconds")
    );
  }
}