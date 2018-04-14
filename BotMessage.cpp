#include "BotMessage.h"
#include <Arduino.h>
#include <ArduinoJson.h>

BotMessage::BotMessage()
    : command("")
    , data("")
{
}

BotMessage::BotMessage(String json)
{
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.parseObject(json);
    command = (const char*)root["command"];
    data = (const char*)root["data"];

    JsonArray& pinsArray = root["pins"];
    pinsArray.copyTo(pins);
}

String BotMessage::getCommand()
{
    return command;
}

String BotMessage::getData()
{
    return data;
}

int* BotMessage::getPins()
{
    return pins;
}

void BotMessage::setCommand(String command)
{
    this->command = command;
}

void BotMessage::setData(String data)
{
    this->data = data;
}

void BotMessage::setPins(int pins[PinsArrayLength])
{
    for (int i = 0; i < PinsArrayLength; i++) {
        this->pins[i] = pins[i];
    }
}

String BotMessage::serialize()
{
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.createObject();
    root["command"] = command;
    root["data"] = data;

    JsonArray& pinsArray = root.createNestedArray("pins");
    pinsArray.copyFrom(pins);

    String json;
    root.printTo(json);

    return json;
}