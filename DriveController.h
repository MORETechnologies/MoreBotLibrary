#ifndef DriveController_h
#define DriveController_h

#include <Arduino.h>

class DriveController
{
public:
    DriveController(int speed, int leftSpeedPin, int leftDirectionPin1, int leftDirectionPin2, int rightSpeedPin, int rightDirectionPin1, int rightDirectionPin2);
    void setup();
    void goForward();
    void goBackward();
    void turnLeft();
    void turnRight();
    void moveLeftForward();
    void moveRightForward();
    void moveLeftBackward();
    void moveRightBackward();
    int getSpeed();
    void setSpeed(int speed);

private:
    int speed;
    int mappedSpeed;
    int leftSpeedPin;
    int leftDirectionPin1;
    int leftDirectionPin2;
    int rightSpeedPin;
    int rightDirectionPin1;
    int rightDirectionPin2;
};

#endif
