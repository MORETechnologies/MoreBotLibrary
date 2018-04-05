#include "BotMessage.h"
#include "WifiProcessor.h"
#include "UltrasonicProcessor.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define enA 5
#define in1 4
#define in2 3

#define enB 6
#define in3 7
#define in4 8
/*
  Left motor is A
  Right motor is B
   _______
   HIGH   | Backward
   LOW    |
   -------

   _______
   LOW   | Forward
   HIGH  |
   -------
*/
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

const int LedPin = 13;

WifiProcessor wifi(WifiRxPin, WifiTxPin);
UltrasonicProcessor sonic(TriggerPin, EchoPin);

int moveMode = 0;
int moveSpeed = 50;

void setup()
{
    Serial.begin(SerialBaudRate);
    wifi.begin("MORETechCo");

    sonic.setup();

    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    pinMode(enB, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    // Set initial rotation direction
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

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
                forward(differenceToTarget * YoyoSpeedMultiplier);
            }
        } else {
            forward(0);
        }
    } else if (command != "") {
        if (data == "stop") {
            digitalWrite(LedPin, LOW);
            forward(0);
        } else if (data == "forward") {
            digitalWrite(LedPin, HIGH);
            forward(moveSpeed);
        } else if (data == "backward") {
            digitalWrite(LedPin, HIGH);
            backward(moveSpeed);
        } else if (data == "right") {
            digitalWrite(LedPin, HIGH);
            right(moveSpeed);
        } else if (data == "left") {
            digitalWrite(LedPin, HIGH);
            left(moveSpeed);
        }
    }

    delay(10);
}

/*
   Driving functions
   Inputs: speed 0 - 100
   Example calls:
   left(100);
   forward(23);
   right(val);
   backward(0);
*/
void right(int vel)
{
    vel = map(vel, 0, 100, 0, 255);
    //Left forwards
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    //Right backwards
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    //Tell them how fast to go (vel -> speed)
    analogWrite(enA, vel);
    analogWrite(enB, vel);
}

void left(int vel)
{
    vel = map(vel, 0, 100, 0, 255);
    //Left backwards
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    //Right forwards
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    //Tell them how fast to go (vel -> speed)
    analogWrite(enA, vel);
    analogWrite(enB, vel);
}

void forward(int vel)
{
    if (vel > 100)
        vel = 100;
    vel = map(vel, 0, 100, 0, 255);
    if (vel > 0) {
        //Left forwards
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);

        //Right forwards
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);

        //Tell them how fast to go (vel -> speed)
        analogWrite(enA, vel);
        analogWrite(enB, vel);
    } else
        backward(-vel);
}

void backward(int vel)
{
    vel = map(vel, 0, 100, 0, 255);
    //Left backwards
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    //Right backwards
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    //Tell them how fast to go (vel -> speed)
    analogWrite(enA, vel);
    analogWrite(enB, vel);
}
