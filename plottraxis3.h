/*
 * PlotterAxis.h - Library for plotter with stepper-motor-driven axis.
 * Created by Mattis Männel aka SparkyCola, October 8, 2015.
 */

#ifndef PlottRAxis3_h
#define PlottRAxis3_h

#include "Arduino.h"
//#include </home/mattis/.arduino-1.6.7/hardware/arduino/avr/cores/arduino/Arduino.h>
//#include "/home/mattis/Coding/Qt/Arduino/CustomLibs/hardware/arduino/avr/cores/arduino/Arduino.h"
class PlottRAxis3
{
public:
    PlottRAxis3(int NumberOfSteps, int DirectionPin,int StepPin, int SleepPin, int ForklightPin);
    long getPosition();
    void setPosition(long PositionToMove);
    int move(long StepsToMove);
    void reset();
    void setSpeed(long Speed);   //speed in RPMs
    long getSpeed();
    void setMaximum(long Maximum);
    long getMaximum();
    void setMinimum(long Minimum);
    void setMicrostepping(int MicrosteppingFactor);
    String getMicroStepping();
    void enableMicrostepping(bool EnableMicrostepping);
    void enableReversedDircetion(bool Reverse);
    void sleep();
    void wakeUp();
    bool isSleeping();
    int version();

private:
    void step();
    void setDirection(bool Direction);

    int stepNumber, numberOfSteps, microsteppingFactor, steppingFactor;
    byte directionPin, stepPin, sleepPin, forklightPin;
    long speed;
    unsigned int stepDelayTime;
    int resolution;
    long maximum, minimum, position;
    bool sleeping, disableMicrostepping, reverseDirection;

    //bool direction;
};

#endif
