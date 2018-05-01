#ifndef BotMessage_h
#define BotMessage_h

#include <Arduino.h>

class BotMessage
{
public:
    BotMessage();
    BotMessage(String json);
    String getCommand();
    String getData();
    void setCommand(String command);
    void setData(String data);
    String serialize();

protected:
    String command;
    String data;
};

#endif
