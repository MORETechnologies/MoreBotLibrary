#include "WifiProcessor.h";
#include "BotMessage.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

const long WifiProcessor::InitialBaudRate = 115200;
const int WifiProcessor::BaudRate = 9600;
const int WifiProcessor::ResponseTimeout = 300;

WifiProcessor::WifiProcessor(int rxPin, int txPin)
    : wifi(rxPin, txPin)
{
    currentStep = ReadStep::Search;
}

void WifiProcessor::begin(String name)
{
    // Start with default wifi baud rate in case it has never been changed
    wifi.begin(InitialBaudRate);

    // Set baud rate
    wifi.println("AT+UART=9600,8,1,0,0");

    // Switching to new baud rate
    wifi.end();
    wifi.begin(BaudRate);

    // Set AP ssid
    wifi.println("AT+CWSAP=\"" + name + "\",\"\",5,0");
    // Clear buffer
    getResponse();

    // Set as access point and station
    wifi.println("AT+CWMODE=3");
    // Clear buffer
    getResponse();

    // Allow multiple connections
    wifi.println("AT+CIPMUX=1");
    // Clear buffer
    getResponse();

    // Create tcp server on port 333
    wifi.println("AT+CIPSERVER=1,333");
    // Clear buffer
    getResponse();
}

// Read in the wifi buffer.
// Doing it in steps like this allows part of the message to be received
// and not stall until the full message is received
void WifiProcessor::read()
{
    // This will be set to true when read to delimiter reaches the delimiter
    bool stepComplete = false;

    if (currentStep == ReadStep::Search) {
        readToDelimiter('+', stepComplete);

        if (stepComplete) {
            currentStep = ReadStep::CommandType;
            messageBuffer = "";
        }
    }

    if (currentStep == ReadStep::CommandType) {
        messageBuffer += readToDelimiter(',', stepComplete);

        if (stepComplete) {
            if (messageBuffer == "IPD") {
                currentStep = ReadStep::ConnectionId;
            } else {
                currentStep = ReadStep::Search;
            }
        }
    }

    if (currentStep == ReadStep::ConnectionId) {
        readToDelimiter(',', stepComplete);

        if (stepComplete) {
            currentStep = ReadStep::DataLength;
            messageBuffer = "";
        }
    }

    if (currentStep == ReadStep::DataLength) {
        messageBuffer += readToDelimiter(':', stepComplete);

        if (stepComplete) {
            currentStep = ReadStep::Input;
            dataLength = messageBuffer.toInt();
            messageBuffer = "";
        }
    }

    if (currentStep == ReadStep::Input) {
        unsigned long startTime = millis();

        while (messageBuffer.length() < dataLength && millis() - startTime < ResponseTimeout) {
            if (wifi.available()) {
                messageBuffer += (char)wifi.read();
            }
        }

        if (messageBuffer.length() == dataLength) {
            currentStep = ReadStep::Search;
            message = BotMessage(messageBuffer);
        }
    }
}

bool WifiProcessor::isMessageAvailable()
{
    return message.getCommand() != "";
}

// Not an actual queue right now
BotMessage WifiProcessor::dequeueMessage()
{
    BotMessage currentMessage = message;
    message = BotMessage();

    return currentMessage;
}

String WifiProcessor::getResponse()
{
    String response = "";

    unsigned long startTime = millis();
    while (millis() - startTime < ResponseTimeout) {
        while (wifi.available()) {
            char character = wifi.read();
            response += character;
        }
    }

    return response;
}

String WifiProcessor::readToDelimiter(char delimiter, bool& doesReach)
{
    String data = "";
    doesReach = false;
    char lastCharacter = '\0';
    unsigned long startTime = millis();

    while (lastCharacter != delimiter && millis() - startTime < ResponseTimeout) {
        if (wifi.available()) {
            lastCharacter = wifi.read();
            data += lastCharacter;
        }
    }

    if (lastCharacter == delimiter) {
        data.remove(data.length() - 1);
        doesReach = true;
    }

    return data;
}