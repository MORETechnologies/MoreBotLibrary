#ifndef BotMessage_h
#define BotMessage_h

#include <Arduino.h>
#include <ArduinoJson.h>

class BotMessage
{
public:
    static const int PinsArrayLength = 3;

    BotMessage();
    BotMessage(String json);
    String getCommand();
    String getData();
    int* getPins();
    void setCommand(String command);
    void setData(String data);
    void setPins(int pins[PinsArrayLength]);
    String serialize();

private:
    String command;
    String data;
    int pins[PinsArrayLength];
};

#endif
