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
            all
        };

        void clearProgram(Program program);
        void requestProgram(Program program);
        bool hasAnyProgram() { return programOneCup || programTwoCup; }
  
        ProgramComponent(HomieNode & node) : senseoNode(node) {}
    private:
        HomieNode & senseoNode; 
        bool programOneCup = false;
        bool programTwoCup = false;
};