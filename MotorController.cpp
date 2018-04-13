#include "MotorController.h";
#include <Arduino.h>

MotorController::MotorController(int speedPin, int directionPin1, int directionPin2)
    : speedPin(speedPin)
    , directionPin1(directionPin1)
    , directionPin2(directionPin2)
{
}

void MotorController::setup()
{
    pinMode(speedPin, OUTPUT);
    pinMode(directionPin1, OUTPUT);
    pinMode(directionPin2, OUTPUT);
}

void MotorController::rotateClockwise(int speed)
{
    speed = constrain(speed, 0, 100);
    speed = map(speed, 0, 100, 0, 255);

    digitalWrite(directionPin1, LOW);
    digitalWrite(directionPin2, HIGH);
    analogWrite(speedPin, speed);
}

void MotorController::rotateCounterclockwise(int speed)
{
    speed = constrain(speed, 0, 100);
    speed = map(speed, 0, 100, 0, 255);

    digitalWrite(directionPin1, HIGH);
    digitalWrite(directionPin2, LOW);
    analogWrite(speedPin, speed);
}