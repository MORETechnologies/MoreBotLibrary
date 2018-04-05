#ifndef UltrasonicProcessor_h
#define UltrasonicProcessor_h

#include <Arduino.h>

class UltrasonicProcessor
{
public:
    UltrasonicProcessor(int triggerPin, int echoPin);
    double readDistance();

private:
    static const int SampleCount = 10;
    static const int ReadTimeout;
    int triggerPin;
    int echoPin;
    double distances[SampleCount];
    int currentIndex;
};

#endif
