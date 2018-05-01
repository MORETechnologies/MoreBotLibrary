#include "PinMessage.h"
#include <Arduino.h>
#include <ArduinoJson.h>

PinMessage::PinMessage(String json)
{
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.parseObject(json);
    command = (const char*)root["command"];
    data = (const char*)root["data"];
    pinNumber = root["pinNumber"];
}

byte PinMessage::getPinNumber()
{
    return pinNumber;
}

byte PinMessage::getPinValue()
{
    return data.toInt();
}

String PinMessage::serialize()
{
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.createObject();
    root["command"] = command;
    root["data"] = data;
    root["pinNumber"] = pinNumber;

    String json;
    root.printTo(json);

    return json;
}