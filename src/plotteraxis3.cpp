/*
 * PlotterAxis.cpp - Library for plotter with stepper-motor-driven axis.
 * Created by Mattis Männel aka SparkyCola, October 8, 2015.
 */

//only for autocomplete & hightlighting in qt
#if QT_VERSION <= 0x040000

//#include "/home/mattis/.arduino-1.6.7/hardware/arduino/avr/cores/arduino/Arduino.h"
#endif

#include "plotteraxis3.h"

PlotterAxis3::PlotterAxis3(int NumberOfSteps, int DirectionPin,int StepPin, int SleepPin, int ForklightPin)
{
    numberOfSteps = NumberOfSteps;
    directionPin = DirectionPin;
    stepPin = StepPin;
    sleepPin = SleepPin;
    forklightPin = ForklightPin;

    position = 0;
    maximum = 0;
    setSpeed(500);
    setMaximum(2000);
    disableMicrostepping = true;
    setMicrostepping(1);

    pinMode(DirectionPin, OUTPUT);
    pinMode(StepPin, OUTPUT);
    pinMode(SleepPin, OUTPUT);

    digitalWrite(SleepPin, LOW);   //sleep
    sleeping = true;

    pinMode(ForklightPin, INPUT_PULLUP);       //set forklightPin as input with pull-up resitors
    //digitalWrite(forklightPin, HIGH);   //activate pull-up resistors for forklightPin
}

long PlotterAxis3::getPosition()
{
    return position;
}

void PlotterAxis3::setPosition(long PositionToMove)
{
    Serial.print("...");
    if(PositionToMove >= 0)
        for(long i = 0; i < PositionToMove; i++){
            move(1);
            Serial.println(i+1);
        }
    else
        for(long i = 0; i < PositionToMove * -1; i++){
            move(-1);
            Serial.println(i+1);
        }
    Serial.println("{done}");
    //position = position_to_move;
}

int PlotterAxis3::move(long StepsToMove)
{
    if(position + StepsToMove > maximum || position + StepsToMove < 0)
        return 1;
    //StepsToMove = StepsToMove * steppingFactor;
    Serial.print("\nstepsToMove: ");  Serial.println(StepsToMove);
    if(StepsToMove >= 0) //fwd
    {
        setDirection(0);
        for(long i = 0; i < StepsToMove; i++)
        {
            step();
            position++;
            delay(stepDelayTime);
        }
    }
    else    //rwd
    {
        StepsToMove = -StepsToMove;
        setDirection(1);
        for(long i = 0; i < StepsToMove; i++)
        {
            step();
            position--;
            delay(stepDelayTime);
        }
    }

    return 0;
}

void PlotterAxis3::reset()
{
    wakeUp();
    setDirection(1);
    while(!digitalRead(forklightPin))
    {
        step();
        delay(stepDelayTime);
    }
    position = 0;
    sleep();
}

void PlotterAxis3::setSpeed(long Speed)
{
    speed = Speed;
    stepDelayTime = 60L * 1000L / (long)(numberOfSteps * speed * microsteppingFactor);
}

long PlotterAxis3::getSpeed()
{
    return speed;
}

long PlotterAxis3::getMaximum()
{
    return maximum;
}

void PlotterAxis3::setMaximum(long Maximum)
{
    maximum = Maximum;
}

void PlotterAxis3::setMicrostepping(int MicrosteppingFactor)
{
    microsteppingFactor = MicrosteppingFactor;
    stepDelayTime = 60L * 1000L / (long)(numberOfSteps * speed * microsteppingFactor);
    if(disableMicrostepping)
        steppingFactor = microsteppingFactor;
    Serial.print("µSteppingFactor: ");  Serial.println(microsteppingFactor);
    Serial.print("steppingFactor: ");  Serial.println(steppingFactor);
}

void PlotterAxis3::enableMicrostepping(bool EnableMicrostepping)
{
    disableMicrostepping = !EnableMicrostepping;
    if(disableMicrostepping)
        steppingFactor = microsteppingFactor;
    else
        steppingFactor = 0;
}

void PlotterAxis3::sleep()
{
    digitalWrite(sleepPin, LOW);    //sleep on
    delayMicroseconds(100);
    sleeping = true;
}

void PlotterAxis3::wakeUp()
{
    digitalWrite(sleepPin, HIGH);   //sleep off
    delayMicroseconds(100);
    sleeping = false;
}

bool PlotterAxis3::isSleeping()
{
    return sleeping;
}

int PlotterAxis3::version()
{
    return 1;
}

void PlotterAxis3::step()
{
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
}

void PlotterAxis3::setDirection(bool Direction)
{
    digitalWrite(directionPin, Direction);
    delayMicroseconds(100);
}
