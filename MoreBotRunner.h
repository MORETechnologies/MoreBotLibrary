#ifndef MoreBotRunner_h
#define MoreBotRunner_h

#include "DriveController.h"
#include "UltrasonicProcessor.h"
#include "WifiProcessor.h"
#include <Arduino.h>

class MoreBotRunner
{
public:
    MoreBotRunner();
    void setup(String wifiName);
    void run();

private:
    static const int WifiRxPin;
    static const int WifiTxPin;

    // Ultrasonic pins
    static const int TriggerPin;
    static const int EchoPin;
    // Ultrasonic settings
    static const double TargetDistance;
    static const double MinimumDistance;
    static const double MaximumDistance;
    static const int YoyoSpeedMultiplier;

    // Motor pins
    static const int LeftSpeedPin;
    static const int LeftDirectionPin1;
    static const int LeftDirectionPin2;
    static const int RightSpeedPin;
    static const int RightDirectionPin1;
    static const int RightDirectionPin2;

    int moveMode;
    int moveSpeed;

    WifiProcessor wifi;
    UltrasonicProcessor sonic;
    DriveController driver;
};

#endif
