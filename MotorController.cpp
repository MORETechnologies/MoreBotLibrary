#include "MotorController.h"
#include "BotMessage.h"
#include <Arduino.h>

MotorController::MotorController()
    : id(-1)
    , speedPin(0)
    , directionPin1(0)
    , directionPin2(0)
{
}

MotorController::MotorController(int speedPin, int directionPin1, int directionPin2)
    : id(-1)
    , speedPin(speedPin)
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

// indexes: speedPin = 0, directionPin1 = 1, directionPin2 = 2
void MotorController::setup(BotMessage& botMessage)
{
    id = botMessage.getId();

    int* pins = botMessage.getPins();

    speedPin = pins[0];
    directionPin1 = pins[1];
    directionPin2 = pins[2];

    setup();
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

int MotorController::getId()
{
    return id;
}

void MotorController::setId(int id)
{
    this->id = id;
}