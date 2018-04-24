#ifndef MotorMessage_h
#define MotorMessage_h

#include "BotMessage.h"
#include "MotorMessage.h"
#include <Arduino.h>
#include <ArduinoJson.h>

class MotorMessage : public BotMessage
{
public:
    MotorMessage();
    MotorMessage(String json);
    int getSpeedPin();
    int getDirectionPin1();
    int getDirectionPin2();
    int getId();
    int getSpeed();
    void setPins(int speedPin, int directionPin1, int directionPin2);
    void setId(int id);
    String serialize();

private:
    int id;
    int speedPin;
    int directionPin1;
    int directionPin2;
};

#endif
