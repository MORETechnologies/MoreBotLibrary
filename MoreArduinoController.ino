#include "BotMessage.h"
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

const long InitialBaudRate = 115200;
const int BaudRate = 9600;
const int WifiRxPin = 12;
const int WifiTxPin = 13;
const char NoResponse[] = "NA";

const int ResponseTimeout = 50;
const int InputLengthIndex = 6;

SoftwareSerial wifi(WifiRxPin, WifiTxPin);

void setup()
{
    wifi.begin(InitialBaudRate);
    Serial.begin(BaudRate);

    // Set baud rate
    wifi.println("AT+UART=9600,8,1,0,0");

    // Switching to new baud rate
    wifi.end();
    wifi.begin(BaudRate);

    // Set AP ssid
    wifi.println("AT+CWSAP=\"moretech\",\"\",5,0");
    Serial.println(getWifiResponse(wifi, ResponseTimeout));

    // Set as access point and station
    wifi.println("AT+CWMODE=3");
    Serial.println(getWifiResponse(wifi, ResponseTimeout));

    // Allow multiple connections
    wifi.println("AT+CIPMUX=1");
    Serial.println(getWifiResponse(wifi, ResponseTimeout));

    // Create tcp server on port 333
    wifi.println("AT+CIPSERVER=1,333");
    Serial.println(getWifiResponse(wifi, ResponseTimeout));

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
}

double dist;
double targetDist = 20.0;
// 0 = auto, 1 = manual, 2 = roomba
int moveMode = 0;
int moveSpeed = 50;

void loop()
{
    String response = "";
    char character = getChar(wifi);
    while (character != '+') {
        response += character;
        character = getChar(wifi);
    }

    Serial.println(response);

    response = readInput(wifi);

    Serial.println(response);

    BotMessage message(response);
    String command = message.getCommand();
    String data = message.getData();

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
            if (abs(error) > 1 && abs(error) < 20)
                forward(-error * 5);
        } else
            forward(0);
    } else {
        if (data == "stop") {
            forward(0);
        } else if (data == "forward")
            forward(moveSpeed);
        else if (data == "backward")
            backward(moveSpeed);
        else if (command == "right")
            right(moveSpeed);
        else if (command == "left")
            left(moveSpeed);
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

void forwardLeft(int vel)
{
    vel = map(vel, 0, 100, 0, 255);

    //Left forwards
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    //Right forwards
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    //Tell them how fast to go (vel -> speed)
    analogWrite(enA, vel / 3);
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
    if (count > 0 && sum > 0)
        distance = sum / count;
    // Prints the distance on the Serial Monitor
    if (distance > 100)
        distance = 0;
    //Serial.print("Distance: ");
    //Serial.println(distance);

    return distance;
}

String getWifiResponse(SoftwareSerial& wifi, int timeout)
{
    String response = "";

    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
        while (wifi.available()) {
            char character = wifi.read();
            response += character;
        }
    }

    if (response == "") {
        response = NoResponse;
    }

    response.trim();
    return response;
}

String readInput(SoftwareSerial& wifi)
{
    String command = "";
    String connectionId = "";
    long dataLength = 0;
    String input = "";
    char character = ' ';

    // Read command type
    while (character != ',') {
        character = getChar(wifi);
        command += character;
    }

    // Read connection id
    character = ' ';
    while (character != ',') {
        character = getChar(wifi);
        connectionId += character;
    }

    // Read data length
    dataLength = wifi.parseInt();

    // Read in semicolon
    getChar(wifi);

    // Read data
    while (dataLength > 0) {
        character = getChar(wifi);
        input += character;
        dataLength--;
    }

    return input;
}

char getChar(SoftwareSerial& wifi)
{
    while (!wifi.available()) {
        delay(10);
    }

    return wifi.read();
}
