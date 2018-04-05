#include "DriveController.h";
#include <Arduino.h>

DriveController::DriveController(int leftSpeedPin, int leftDirectionPin1, int leftDirectionPin2, int rightSpeedPin, int rightDirectionPin1, int rightDirectionPin2)
    : leftSpeedPin(leftSpeedPin)
    , leftDirectionPin1(leftDirectionPin1)
    , leftDirectionPin2(leftDirectionPin2)
    , rightSpeedPin(rightSpeedPin)
    , rightDirectionPin1(rightDirectionPin1)
    , rightDirectionPin2(rightDirectionPin2)
{
}

void DriveController::setup()
{
    pinMode(leftSpeedPin, OUTPUT);
    pinMode(leftDirectionPin1, OUTPUT);
    pinMode(leftDirectionPin2, OUTPUT);

    pinMode(rightSpeedPin, OUTPUT);
    pinMode(rightDirectionPin1, OUTPUT);
    pinMode(rightDirectionPin2, OUTPUT);
}

void DriveController::goForward(int speed)
{
    moveLeftForward(speed);
    moveRightForward(speed);
}

void DriveController::goBackward(int speed)
{
    moveLeftBackward(speed);
    moveRightBackward(speed);
}

void DriveController::turnLeft(int speed)
{
    moveLeftBackward(speed);
    moveRightForward(speed);
}

void DriveController::turnRight(int speed)
{
    moveLeftForward(speed);
    moveRightBackward(speed);
}

void DriveController::moveLeftForward(int speed)
{
    speed = constrain(speed, 0, 100);
    speed = map(speed, 0, 100, 0, 255);

    digitalWrite(leftDirectionPin1, LOW);
    digitalWrite(leftDirectionPin2, HIGH);
    analogWrite(leftSpeedPin, speed);
}

void DriveController::moveRightForward(int speed)
{
    speed = constrain(speed, 0, 100);
    speed = map(speed, 0, 100, 0, 255);

    digitalWrite(rightDirectionPin1, LOW);
    digitalWrite(rightDirectionPin2, HIGH);
    analogWrite(rightSpeedPin, speed);
}

void DriveController::moveLeftBackward(int speed)
{
    speed = constrain(speed, 0, 100);
    speed = map(speed, 0, 100, 0, 255);

    digitalWrite(leftDirectionPin1, HIGH);
    digitalWrite(leftDirectionPin2, LOW);
    analogWrite(leftSpeedPin, speed);
}

void DriveController::moveRightBackward(int speed)
{
    speed = constrain(speed, 0, 100);
    speed = map(speed, 0, 100, 0, 255);

    digitalWrite(rightDirectionPin1, HIGH);
    digitalWrite(rightDirectionPin2, LOW);
    analogWrite(rightSpeedPin, speed);
}