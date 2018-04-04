#ifndef BotMessage_h
#define BotMessage_h

#include <Arduino.h>
#include <ArduinoJson.h>

class BotMessage
{
public:
    BotMessage();
    BotMessage(String json);
    BotMessage(String command, String data);
    String getCommand();
    String getData();
    void setCommand(String command);
    void setData(String data);
    String serialize();

private:
    String command;
    String data;
};

#endif
