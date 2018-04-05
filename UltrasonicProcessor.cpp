#include "UltrasonicProcessor.h";
#include <Arduino.h>

const int UltrasonicProcessor::ReadTimeout = 100;

UltrasonicProcessor::UltrasonicProcessor(int triggerPin, int echoPin)
    : currentIndex(0)
    , triggerPin(triggerPin)
    , echoPin(echoPin)
{
    for (int i = 0; i < SampleCount; i++) {
        distances[i] = 0;
    }
}

double UltrasonicProcessor::readDistance()
{
    // To make sure pin is not HIGH
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);

    // Make it HIGH for 10 microseconds to send the sound
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Not sure what the timeout should be
    // See how long it takes for the sound to come back
    double duration = pulseIn(echoPin, HIGH, ReadTimeout);
    double distance = distance = duration * 0.034 / 2;

    // For calculating the average of last 10 reads to reduce jerkiness
    distances[currentIndex] = distance;
    currentIndex = (currentIndex + 1) % SampleCount;

    double total = 0;
    for (int i = 0; i < SampleCount; i++) {
        total += distances[i];
    }

    double average = total / SampleCount;

    return average;
}