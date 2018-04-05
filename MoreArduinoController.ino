#include "BotMessage.h"
#include "DriveController.h"
#include "UltrasonicProcessor.h"
#include "WifiProcessor.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

const int SerialBaudRate = 9600;

const int WifiRxPin = 12;
const int WifiTxPin = 2;

// Ultrasonic pins
const int TriggerPin = 10;
const int EchoPin = 11;
// Ultrasonic settings
const double TargetDistance = 20;
const double MinimumDistance = 1;
const double MaximumDistance = 50;
const int YoyoSpeedMultiplier = 5;

// Motor pins
const int LeftSpeedPin = 5;
const int LeftDirectionPin1 = 4;
const int LeftDirectionPin2 = 3;
const int RightSpeedPin = 6;
const int RightDirectionPin1 = 7;
const int RightDirectionPin2 = 8;

const int LedPin = 13;

int moveMode = 0;
int moveSpeed = 50;

WifiProcessor wifi(WifiRxPin, WifiTxPin);
UltrasonicProcessor sonic(TriggerPin, EchoPin);
DriveController driver(LeftSpeedPin, LeftDirectionPin1, LeftDirectionPin2, RightSpeedPin, RightDirectionPin1, RightDirectionPin2);

void setup()
{
    Serial.begin(SerialBaudRate);
    wifi.begin("MORETechCo");

    sonic.setup();
    driver.setup();

    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);
}

void loop()
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
            digitalWrite(LedPin, LOW);
            driver.goForward(0);
        } else if (data == "forward") {
            digitalWrite(LedPin, HIGH);
            driver.goForward(moveSpeed);
        } else if (data == "backward") {
            digitalWrite(LedPin, HIGH);
            driver.goBackward(moveSpeed);
        } else if (data == "right") {
            digitalWrite(LedPin, HIGH);
            driver.turnRight(moveSpeed);
        } else if (data == "left") {
            digitalWrite(LedPin, HIGH);
            driver.turnLeft(moveSpeed);
        }
    }

    delay(10);
}