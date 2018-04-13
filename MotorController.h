#ifndef MotorController_h
#define MotorController_h

#include <Arduino.h>

class MotorController
{
public:
    MotorController(int speedPin, int directionPin1, int directionPin2);
    void setup();
    void rotateClockwise(int speed);
    void rotateCounterclockwise(int speed);

private:
    int speedPin;
    int directionPin1;
    int directionPin2;
};

#endif
