#ifndef PinMessageProcessor_h
#define PinMessageProcessor_h

#include "BotMessage.h"
#include <Arduino.h>

class PinMessageProcessor
{
public:
    static void process(BotMessage& message);

private:
    PinMessageProcessor() {}
};

#endif
