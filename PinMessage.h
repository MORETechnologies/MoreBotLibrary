#ifndef PinMessage_h
#define PinMessage_h

#include "BotMessage.h"
#include <Arduino.h>

class PinMessage : public BotMessage
{
public:
    PinMessage(String json);
    byte getPinNumber();
    byte getPinValue();
    String serialize();

private:
    byte pinNumber;
};

#endif
