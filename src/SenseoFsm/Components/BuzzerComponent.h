#pragma once

#include <ezBuzzer.h>
#include "ModularFsm/FsmComponent.h"

class BuzzerComponent : public  FsmComponent<BuzzerComponent>
{
    public:
        BuzzerComponent(int buzzerPin) : myBuzzer(buzzerPin) {}
        bool playMelody(const String & tune);
        void update();
        void beep();

        static const char * getValidTunes();

    private:
        ezBuzzer myBuzzer;
};