#ifndef MotorProcessor_h
#define MotorProcessor_h

#include "BotMessage.h"
#include "MotorController.h"
#include <Arduino.h>

class MotorProcessor
{
public:
    static const int MotorsCount = 8;
    void process(BotMessage& message);

private:
    MotorController motors[MotorsCount];
};

#endif
