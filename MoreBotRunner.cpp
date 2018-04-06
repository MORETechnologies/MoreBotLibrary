#include "MoreBotRunner.h"
#include "BotMessage.h"
#include "DriveController.h"
#include "UltrasonicProcessor.h"
#include "WifiProcessor.h"
#include <Arduino.h>

const int MoreBotRunner::WifiRxPin = 12;
const int MoreBotRunner::WifiTxPin = 2;

// Ultrasonic pins
const int MoreBotRunner::TriggerPin = 10;
const int MoreBotRunner::EchoPin = 11;
// Ultrasonic settings
const double MoreBotRunner::TargetDistance = 20;
const double MoreBotRunner::MinimumDistance = 1;
const double MoreBotRunner::MaximumDistance = 50;
const int MoreBotRunner::YoyoSpeedMultiplier = 5;

// Motor pins
const int MoreBotRunner::LeftSpeedPin = 5;
const int MoreBotRunner::LeftDirectionPin1 = 4;
const int MoreBotRunner::LeftDirectionPin2 = 3;
const int MoreBotRunner::RightSpeedPin = 6;
const int MoreBotRunner::RightDirectionPin1 = 7;
const int MoreBotRunner::RightDirectionPin2 = 8;

MoreBotRunner::MoreBotRunner()
    : moveMode(0)
    , moveSpeed(50)
    , wifi(WifiRxPin, WifiTxPin)
    , sonic(TriggerPin, EchoPin)
    , driver(LeftSpeedPin, LeftDirectionPin1, LeftDirectionPin2, RightSpeedPin, RightDirectionPin1, RightDirectionPin2)
{
}

void MoreBotRunner::setup(String wifiName)
{
    wifi.begin(wifiName);

    sonic.setup();
    driver.setup();
}

void MoreBotRunner::run()
{
    String command = "";
    String data = "";

    wifi.read();
    if (wifi.isMessageAvailable()) {
        BotMessage message = wifi.dequeueMessage();
        command = message.getCommand();
        data = message.getData();
    }

    if (command == "mode") {
        if (data == "yoyo") {
            moveMode = 0;
        } else {
            moveMode = 1;
        }
    }

    if (moveMode == 0) {
        double distance = sonic.readDistance();
        if (distance > 0 && distance <= MaximumDistance) {
            double differenceToTarget = distance - TargetDistance;
            double absoluteDifference = abs(differenceToTarget);
            if (absoluteDifference >= MinimumDistance && absoluteDifference <= TargetDistance) {
                if (differenceToTarget >= 0) {
                    driver.goForward(absoluteDifference * YoyoSpeedMultiplier);
                } else {
                    driver.goBackward(absoluteDifference * YoyoSpeedMultiplier);
                }
            }
        } else {
            driver.goForward(0);
        }
    } else if (command != "") {
        if (data == "stop") {
            driver.goForward(0);
        } else if (data == "forward") {
            driver.goForward(moveSpeed);
        } else if (data == "backward") {
            driver.goBackward(moveSpeed);
        } else if (data == "right") {
            driver.turnRight(moveSpeed);
        } else if (data == "left") {
            driver.turnLeft(moveSpeed);
        }
    }
}