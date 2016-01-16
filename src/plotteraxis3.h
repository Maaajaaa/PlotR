/*
 * PlotterAxis.h - Library for plotter with stepper-motor-driven axis.
 * Created by Mattis Männel aka SparkyCola, October 8, 2015.
 */

#ifndef PlotterAxis3_h
#define PlotterAxis3_h

#include "Arduino.h"
//#include </home/mattis/.arduino-1.6.7/hardware/arduino/avr/cores/arduino/Arduino.h>
//#include "/home/mattis/Coding/Qt/Arduino/CustomLibs/hardware/arduino/avr/cores/arduino/Arduino.h"
class PlotterAxis3
{
public:
    PlotterAxis3(int NumberOfSteps, int DirectionPin,int StepPin, int SleepPin, int ForklightPin);
    long getPosition();
    void setPosition(long PositionToMove);
    int move(long StepsToMove);
    void reset();
    void setSpeed(long Speed);   //speed in RPMs
    long getSpeed();
    void setMaximum(long Maximum);
    long getMaximum();
    void setMicrostepping(int MicrosteppingFactor);
    String getMicroStepping();
    void enableMicrostepping(bool EnableMicrostepping);
    void sleep();
    void wakeUp();
    bool isSleeping();
    int version();

private:
    void step();
    void setDirection(bool Direction);

    int stepNumber, numberOfSteps, microsteppingFactor, steppingFactor;
    byte directionPin, stepPin, sleepPin, forklightPin;
    long position, speed;
    unsigned int stepDelayTime;
    int resolution;
    long maximum;
    bool sleeping, disableMicrostepping;

    //bool direction;
};

#endif
