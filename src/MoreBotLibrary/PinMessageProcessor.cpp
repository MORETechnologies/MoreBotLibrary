#include "PinMessageProcessor.h"
#include "BotMessage.h"
#include "PinMessage.h"
#include <Arduino.h>

void PinMessageProcessor::process(BotMessage& message)
{
    PinMessage pinMessage(message.getData());
    String command = pinMessage.getCommand();
    byte pinNumber = pinMessage.getPinNumber();

    if (command == "output") {
        pinMode(pinNumber, OUTPUT);
    } else if (command == "input") {
        pinMode(pinNumber, INPUT);
    } else if (command == "high") {
        digitalWrite(pinNumber, HIGH);
    } else if (command == "low") {
        digitalWrite(pinNumber, LOW);
    } else if (command == "write") {
        analogWrite(pinNumber, pinMessage.getPinValue());
    }
}