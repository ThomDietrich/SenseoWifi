#include "ModularFsm.h"
#include "FsmComponent.h"
#include "FsmState.h"

void ModularFsm::update(unsigned long currentTime) 
{
    lastUpdateMillis = currentTime;
    updateComponents();
    updateFsm();
}

void ModularFsm::updateComponents() 
{
    for(auto & component: components) 
    {
        component.second->update();
    }
}

void ModularFsm::updateFsm() 
{
    if (nextState != nullptr) 
    {
        if (currentState != nullptr) currentState->onExit(nextState->getStateId());
        
        //call state change cb
        onStateChange(currentState,nextState);

        lastStateChangeMillis = lastUpdateMillis;
        nextState->onEnter(currentState ? currentState->getStateId() : INVALID_FSM_CLASS_ID);
        currentState = nextState;
        nextState = nullptr;
    }

    if (currentState != nullptr) currentState->onUpdate();
}

void ModularFsm::changeState(StateId stateId) 
{
    if (currentState != nullptr && currentState->getStateId() != stateId) {
        if (nextState == nullptr || nextState->getStateId() != stateId) {
            nextState = states[stateId].get();
        }
    }
}

bool ModularFsm::isInState(StateId stateId) const 
{
    return stateId == currentState->getStateId(); 
}

void ModularFsm::setInitialState(StateId stateId) 
{
    assert(currentState == nullptr);
    nextState = states[stateId].get();
}