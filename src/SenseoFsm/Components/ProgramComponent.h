/*
  Cup.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#pragma once

#include "ModularFsm/FsmComponent.h"

class HomieNode;

class ProgramComponent : public  FsmComponent<ProgramComponent>
{
    public:
        enum Program
        {
            oneCup,
            twoCup,
        };

        void clearProgram();
        void requestProgram(Program program,bool powerPressed);
        bool hasProgramPending() const { return hasProgram; }
  
        ProgramComponent(HomieNode & node);
    private:
        HomieNode & senseoNode; 
        bool hasProgram = false;
};