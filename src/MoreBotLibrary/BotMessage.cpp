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
}

String BotMessage::getCommand()
{
    return command;
}

String BotMessage::getData()
{
    return data;
}

void BotMessage::setCommand(String command)
{
    this->command = command;
}

void BotMessage::setData(String data)
{
    this->data = data;
}

String BotMessage::serialize()
{
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.createObject();
    root["command"] = command;
    root["data"] = data;

    String json;
    root.printTo(json);

    return json;
}