#include "MotorMessage.h"
#include "BotMessage.h"
#include <Arduino.h>
#include <ArduinoJson.h>

MotorMessage::MotorMessage(String json)
{
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.parseObject(json);
    command = (const char*)root["command"];
    data = (const char*)root["data"];
    id = root["id"];
    speedPin = root["speedPin"];
    directionPin1 = root["directionPin1"];
    directionPin2 = root["directionPin2"];
}

int MotorMessage::getId()
{
    return id;
}

int MotorMessage::getSpeedPin()
{
    return speedPin;
}

int MotorMessage::getDirectionPin1()
{
    return directionPin1;
}

int MotorMessage::getDirectionPin2()
{
    return directionPin2;
}

int MotorMessage::getSpeed()
{
    return data.toInt();
}

void MotorMessage::setPins(int speedPin, int directionPin1, int directionPin2)
{
    this->speedPin = speedPin;
    this->directionPin1 = directionPin1;
    this->directionPin2 = directionPin2;
}

void MotorMessage::setId(int id)
{
    this->id = id;
}

String MotorMessage::serialize()
{
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.createObject();
    root["command"] = command;
    root["data"] = data;
    root["id"] = id;
    root["speedPin"] = speedPin;
    root["directionPin1"] = directionPin1;
    root["directionPin2"] = directionPin2;

    String json;
    root.printTo(json);

    return json;
}