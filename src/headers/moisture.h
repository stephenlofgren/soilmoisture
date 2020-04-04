#ifndef MOISTURE_H
    #define MOISTURE_H

    #include "headers/blynk.h"
    #include "headers/utilities.h"
    #include "headers/moisture.h"
    #include <ver.h>
    #include <string>

    void setSensorPin(int pin);
    void setPowerPin(int pin);
    int takeReading(unsigned int numSamples = 1, unsigned int delayCount = 10);
    float readMoisture();
#endif