#include "BotMessage.h"
#include "WifiProcessor.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define enA 5
#define in1 4
#define in2 3

#define enB 6
#define in3 7
#define in4 8

#define trigger 10
#define echo 11
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

const int BaudRate = 9600;
const int WifiRxPin = 12;
const int WifiTxPin = 2;
const int LedPin = 13;

WifiProcessor wifi(WifiRxPin, WifiTxPin);

void setup()
{
    Serial.begin(BaudRate);
    wifi.begin("MORETechCo");

    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    pinMode(enB, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);

    // Set initial rotation direction
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);
}

double dist;
double targetDist = 20.0;
// 0 = auto, 1 = manual
int moveMode = 0;
int moveSpeed = 50;

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
        dist = getDistance();
        if (dist < 50 && dist > 1) {
            double error = targetDist - dist;
            if (abs(error) > 1 && abs(error) < 20) {
                forward(-error * 5);
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

//Distance function
double getDistance()
{
    double sum = 0.0;
    double distance;
    long duration;
    double maxDelta = 2.0;
    double maxDist = 100;
    double prevDist;
    int count;

    for (size_t i = 0; i < 10; i++) {
        // Clears the trigPin
        digitalWrite(trigger, LOW);
        delayMicroseconds(2);

        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigger, LOW);

        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(echo, HIGH);
        if (duration == 0) {
            i = 10;
        } else {
            // Calculating the distance
            distance = duration * 0.034 / 2;

            if (distance < maxDist) {
                if (count == 0) {
                    prevDist = distance;
                    sum += distance;
                    count++;
                } else if (count > 0 && !(abs(distance - prevDist) >= 20)) {
                    //else if(abs(distance - prevDist) < (maxDelta * prevDist)){
                    //Serial.println(abs(distance - prevDist));
                    sum += distance;
                    prevDist = distance;
                    count++;
                }
            }
        }
    }
    if (count > 0 && sum > 0)
        distance = sum / count;
    // Prints the distance on the Serial Monitor
    if (distance > 100)
        distance = 0;
    //Serial.print("Distance: ");
    //Serial.println(distance);

    return distance;
}
