#include "FsmState.h"
#include "ModularFsm.h"

FsmState::FsmState() = default;
FsmState::~FsmState() = default;

void FsmState::initialize(ModularFsm * fsm)
{
    assert(fsm != nullptr);
    myFsm = fsm;
    onInitialized();
}

void FsmState::changeState(StateId stateId)
{
    assert (myFsm != nullptr);
    myFsm->changeState(stateId);
}

unsigned long FsmState::getTimeInState() const 
{ 
    return myFsm->getTimeInState(); 
}

BaseFsmComponent * FsmState::getComponent(FsmComponentId classId) const
{ 
    return myFsm->getComponent(classId); 
}
