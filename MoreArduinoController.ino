#include <ArduinoJson.h>
#include <SoftwareSerial.h>

const int BaudRate = 9600;
const int WifiRxPin = 2;
const int WifiTxPin = 3;

const int ResponseTimeout = 50;
const int InputLengthIndex = 6;

SoftwareSerial wifi(WifiRxPin, WifiTxPin);
StaticJsonBuffer<200> jsonBuffer;

void setup()
{
    wifi.begin(BaudRate);
    Serial.begin(BaudRate);

    wifi.println("AT+CWSAP=\"moretech\",\"\",5,0"); // set ssid
    Serial.println(getWifiResponse(wifi, ResponseTimeout));

    wifi.println("AT+CWMODE=2"); // configure as access point
    Serial.println(getWifiResponse(wifi, ResponseTimeout));

    wifi.println("AT+CIPMUX=1"); // configure for multiple connections
    Serial.println(getWifiResponse(wifi, ResponseTimeout));

    wifi.println("AT+CIPSERVER=1"); // turn on server on port 333
    Serial.println(getWifiResponse(wifi, ResponseTimeout));
}

void loop()
{
    String response = "";
    char character = getChar(wifi);
    while (character != '+') {
        response += character;
        character = getChar(wifi);
    }

    Serial.print(response);

    response = readInput(wifi);

    Serial.println(response);

    JsonObject& root = jsonBuffer.parseObject(response);
    if (root.success()) {
        Serial.println(root["command"]);
        Serial.println(root["data"]);
    }
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
