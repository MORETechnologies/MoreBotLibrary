#include "DriveController.h";
#include <Arduino.h>

DriveController::DriveController(int speed, int leftSpeedPin, int leftDirectionPin1, int leftDirectionPin2, int rightSpeedPin, int rightDirectionPin1, int rightDirectionPin2)
    : speed(abs(speed))
    , leftSpeedPin(leftSpeedPin)
    , leftDirectionPin1(leftDirectionPin1)
    , leftDirectionPin2(leftDirectionPin2)
    , rightSpeedPin(rightSpeedPin)
    , rightDirectionPin1(rightDirectionPin1)
    , rightDirectionPin2(rightDirectionPin2)
    , mappedSpeed(map(speed, 0, 100, 0, 255))
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

void DriveController::goForward()
{
    moveLeftForward();
    moveRightForward();
}

void DriveController::goBackward()
{
    moveLeftBackward();
    moveRightBackward();
}

void DriveController::turnLeft()
{
    moveLeftBackward();
    moveRightForward();
}

void DriveController::turnRight()
{
    moveLeftForward();
    moveRightBackward();
}

void DriveController::moveLeftForward()
{
    digitalWrite(leftDirectionPin1, LOW);
    digitalWrite(leftDirectionPin2, HIGH);
    analogWrite(leftSpeedPin, mappedSpeed);
}

void DriveController::moveRightForward()
{
    digitalWrite(rightDirectionPin1, LOW);
    digitalWrite(rightDirectionPin2, HIGH);
    analogWrite(rightSpeedPin, mappedSpeed);
}

void DriveController::moveLeftBackward()
{
    digitalWrite(leftDirectionPin1, HIGH);
    digitalWrite(leftDirectionPin2, LOW);
    analogWrite(leftSpeedPin, mappedSpeed);
}

void DriveController::moveRightBackward()
{
    digitalWrite(rightDirectionPin1, HIGH);
    digitalWrite(rightDirectionPin2, LOW);
    analogWrite(rightSpeedPin, mappedSpeed);
}

int DriveController::getSpeed()
{
    return speed;
}

void DriveController::setSpeed(int speed)
{
    this->speed = abs(speed);
    mappedSpeed = map(this->speed, 0, 100, 0, 255);
}