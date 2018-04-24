#include "MotorProcessor.h"
#include "BotMessage.h"
#include "MotorController.h"
#include "MotorMessage.h"
#include <Arduino.h>

void MotorProcessor::process(BotMessage& message)
{
    MotorMessage motorMessage(message.getData());
    String command = motorMessage.getCommand();

    if (command == "add") {
        for (int i = 0; i < MotorsCount; i++) {
            if (motors[i].getId() < 0) {
                motors[i].setId(motorMessage.getId());
                motors[i].setup(motorMessage.getSpeedPin(), motorMessage.getDirectionPin1(), motorMessage.getDirectionPin2());
                break;
            }
        }
    } else if (command == "clockwise") {
        int id = motorMessage.getId();
        for (int i = 0; i < MotorsCount; i++) {
            if (motors[i].getId() == id) {
                motors[i].rotateClockwise(motorMessage.getSpeed());
                break;
            }
        }
    } else if (command == "counter") {
        int id = motorMessage.getId();
        for (int i = 0; i < MotorsCount; i++) {
            if (motors[i].getId() == id) {
                motors[i].rotateCounterclockwise(motorMessage.getSpeed());
                break;
            }
        }
    } else if (command == "remove") {
        int id = motorMessage.getId();
        for (int i = 0; i < MotorsCount; i++) {
            if (motors[i].getId() == id) {
                motors[i].setId(-1);
                break;
            }
        }
    }
}