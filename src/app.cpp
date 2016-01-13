//#include </home/mattis/.arduino-1.6.7/hardware/arduino/avr/cores/arduino/Arduino.h>
//#include "/home/mattis/sketchbook/libraries/PlotterAxis3/PlotterAxis3.h"
//#include "plotteraxis3.h"
#include "Arduino.h"
#include "plotteraxis3.h"
#include "Servo.h"
//#include "/home/mattis/Coding/Qt/Arduino/Servo4QtDuino/src/Servo.h"

//PlotterAxis3(int NumberOfSteps, int DirectionPin,int StepPin, int SleepPin, int ForklightPin);
PlotterAxis3 xAxis(48, 4, 5, 6, 2);
PlotterAxis3 yAxis(48, 7, 8, 9, 3);
Servo zAxis;
String inputString = "";
char inputChar; // Where to store the character read

bool verboseOutput = false;

#define  XmmSt  0.02108333 // mm per Step
#define  YmmSt  0.020875

void diagonal(int lenght, int direction);
int processInputCommand(String inputString);
void help();
void up();
void down();
void moveX(int xSteps);
void moveY(int ySteps);
void moveXY(int xMM, int yMM);
void bresenham(int x1, int y1);
void square(int sideLength);
void ellipse(int a, int b);
void circle(int radius);
void setPixel(int x, int y);
void wakeXYup();
void sleepXY();

void setup()
{
  //while (!Serial);
  Serial.begin(115200);
  Serial.print("Hello user! This is the PlotR with PlotterAxis3 V");
  Serial.println(xAxis.version());
  Serial.println();
  xAxis.setSpeed(1000); //orig 1000
  yAxis.setSpeed(1000);
  xAxis.setMaximum(8500);   //bcs of servo wire
  yAxis.setMaximum(8500);   //top of the paper (in alpha bed)
  xAxis.setMicrostepping(2); //halfstepping (via HW enabled)
  yAxis.setMicrostepping(2);
  zAxis.attach(11);
  zAxis.write(130);
  sleepXY();
}

void loop()
{
  //Serial.available does not return more than 1 on VISduinoboards (and all other boards with cheap USB to Serial chips)
  if(Serial.available() > 0)
  {
    inputChar = Serial.read(); // Read character
    if(inputChar == ')')
    {
      inputString += inputChar;
      if(verboseOutput)
      {
        Serial.print("String: "); Serial.println(inputString);
      }
      processInputCommand(inputString);
      inputString = "";
    } else
      inputString += inputChar; // Add character to string
  }
}

void diagonal(int lenght, int direction = 1)
{
  wakeXYup();
  if (lenght < 0)
  {
    Serial.println("ERROR only positve values are supported (yet) for diagonal"); return;
  }

  //convert millimeter (from input) to steps (for plotting)
  int xSteps = lenght / XmmSt;
  int ySteps = lenght / YmmSt;
  Serial.print("xSteps: "); Serial.print(xSteps); Serial.print("  ySteps: "); Serial.print(ySteps); Serial.print("  direction: "); Serial.print(direction); //DEBUG

  switch(direction)
  {
    case 1: // / bottom to top
      for (int i = 0; i < ySteps; i++)
      {
        yAxis.move(1); //Serial.print("Y"); Serial.println(i); //DEBUG
        if( !(  i == 101 || i== 202 || i== 304 || i== 405 || i== 506 || i== 607 || i== 709 || i== 810 || i== 911 || i== 1012 || i== 1114 || i== 1215 || i== 1316 || i== 1417 || i== 1519 || i== 1620 || i== 1721 || i== 1822 || i== 1924 || i== 2025 || i== 2126 || i== 2227 || i== 2329 || i== 2430 || i== 2531 || i== 2632 || i== 2734 || i== 2835 || i== 2934 || i== 2936  || i== 3037 || i== 3139  || i== 3240 || i== 3341  || i== 3442  || i== 3544  || i== 3645|| i== 3746  || i== 3847  || i== 3949  || i== 4050  || i== 4151  || i== 4252  || i== 4354  || i== 4455  || i== 4556  || i== 4657  || i== 4759 || i== 4860  || i== 4961 || i== 5062  || i== 5164))
        {
          xAxis.move(1); /*Serial.print("X"); Serial.println(i);*/
        }
      }
    break;

    case 2: // / top to bottom
      for (int i = 0; i < ySteps; i++)
      {
        yAxis.move(-1); //Serial.print("Y"); Serial.println(i); //DEBUG
        if( !(  i == 101 || i== 202 || i== 304 || i== 405 || i== 506 || i== 607 || i== 709 || i== 810 || i== 911 || i== 1012 || i== 1114 || i== 1215 || i== 1316 || i== 1417 || i== 1519 || i== 1620 || i== 1721 || i== 1822 || i== 1924 || i== 2025 || i== 2126 || i== 2227 || i== 2329 || i== 2430 || i== 2531 || i== 2632 || i== 2734 || i== 2835 || i== 2934 || i== 2936  || i== 3037 || i== 3139  || i== 3240 || i== 3341  || i== 3442  || i== 3544  || i== 3645|| i== 3746  || i== 3847  || i== 3949  || i== 4050  || i== 4151  || i== 4252  || i== 4354  || i== 4455  || i== 4556  || i== 4657  || i== 4759 || i== 4860  || i== 4961 || i== 5062  || i== 5164))
        {
          xAxis.move(-1); /*Serial.print("X"); Serial.println(i);*/
        }
      }
    break;

    case 3: // \ top to bottom
      for (int i = 0; i < ySteps; i++)
      {
        yAxis.move(-1); //Serial.print("Y"); Serial.println(i); //DEBUG
        if( !(  i == 101 || i== 202 || i== 304 || i== 405 || i== 506 || i== 607 || i== 709 || i== 810 || i== 911 || i== 1012 || i== 1114 || i== 1215 || i== 1316 || i== 1417 || i== 1519 || i== 1620 || i== 1721 || i== 1822 || i== 1924 || i== 2025 || i== 2126 || i== 2227 || i== 2329 || i== 2430 || i== 2531 || i== 2632 || i== 2734 || i== 2835 || i== 2934 || i== 2936  || i== 3037 || i== 3139  || i== 3240 || i== 3341  || i== 3442  || i== 3544  || i== 3645|| i== 3746  || i== 3847  || i== 3949  || i== 4050  || i== 4151  || i== 4252  || i== 4354  || i== 4455  || i== 4556  || i== 4657  || i== 4759 || i== 4860  || i== 4961 || i== 5062  || i== 5164))
        {
          xAxis.move(1); /*Serial.print("X"); Serial.println(i);*/
        }
      }
    break;

    case 4: // \ bottom to top
      for (int i = 0; i < ySteps; i++)
      {
        yAxis.move(1); //Serial.print("Y"); Serial.println(i); //DEBUG
        if( !(  i == 101 || i== 202 || i== 304 || i== 405 || i== 506 || i== 607 || i== 709 || i== 810 || i== 911 || i== 1012 || i== 1114 || i== 1215 || i== 1316 || i== 1417 || i== 1519 || i== 1620 || i== 1721 || i== 1822 || i== 1924 || i== 2025 || i== 2126 || i== 2227 || i== 2329 || i== 2430 || i== 2531 || i== 2632 || i== 2734 || i== 2835 || i== 2934 || i== 2936  || i== 3037 || i== 3139  || i== 3240 || i== 3341  || i== 3442  || i== 3544  || i== 3645|| i== 3746  || i== 3847  || i== 3949  || i== 4050  || i== 4151  || i== 4252  || i== 4354  || i== 4455  || i== 4556  || i== 4657  || i== 4759 || i== 4860  || i== 4961 || i== 5062  || i== 5164))
        {
          xAxis.move(-1); /*Serial.print("X"); Serial.println(i);*/
        }
      }
    break;
  }
  sleepXY();
}

int processInputCommand(String inputString)
{
  int beginBracket = -1;
  int endBracket = -1;
  int commas[9];
  int nextCommaIndex = 0;
  int parameters[10];
  int amountOfParameters = 0;
  bool validMethode = false;
  bool parametersOK = false;

  for (unsigned int i = 0; i < inputString.length(); i++)
  {
    if(inputString.charAt(i) == '(')
      if(beginBracket == -1)
        beginBracket = i;
      else
        return 1; //two ( found

    if(inputString.charAt(i) == ')')
      if(endBracket == -1)
        endBracket = i;
      else
        return 2; //two ) found

    if(inputString.charAt(i) == ',')
    {
      //Serial.print("comma found at ");  Serial.println(i);
      commas[nextCommaIndex] = i;
      if(nextCommaIndex < 9)
        nextCommaIndex++;
      else
      {
        Serial.print("too many commas at");  Serial.println(i);
      }
    }
  }

  if(verboseOutput)
  {
    Serial.print("\nbeginBracket: ");  Serial.println(beginBracket);
    Serial.print("endBracket: ");  Serial.println(endBracket);
  }

  if(beginBracket == -1)
    return 3; //no ( found
  if(endBracket == -1)
    return 4; //no ) found

  if(verboseOutput)
    for (int i = 0; i < nextCommaIndex; i++)
    {
      Serial.print("Comma    ");  Serial.print(i);  Serial.print (" : "); Serial.println(commas[i]);
    }

  if(endBracket - beginBracket > 1) //if arguments given
  {
    if (verboseOutput)
      Serial.print("nextCommaIndex: "); Serial.println(nextCommaIndex);
    for (int i = 0; i < nextCommaIndex; i++)
    {
      if(i == 0) {
        parameters[0] = inputString.substring(beginBracket + 1, commas[0]).toInt();
        amountOfParameters++;
        if(verboseOutput)
        {
          Serial.print("Argument    ");  Serial.print(0);  Serial.print (" : "); Serial.println( parameters[0]);
        }
      } else
      {
        parameters[i] = inputString.substring(commas[i - 1] + 1, commas[i]).toInt();
        amountOfParameters++;
        if(verboseOutput)
        {
          Serial.print("Argument    ");  Serial.print(i);  Serial.print (" : "); Serial.println(parameters[i]);
        }
      }

    }
    if(nextCommaIndex > 0)
    {
      parameters[nextCommaIndex] = inputString.substring(commas[ nextCommaIndex - 1] + 1, endBracket).toInt();
      amountOfParameters++;
      if(verboseOutput) {
        Serial.print("Argument   ");
        Serial.print( nextCommaIndex);
        Serial.print (" : ");
        Serial.println( parameters[nextCommaIndex] );
      }
    }

    if(nextCommaIndex == 0) { //one argument
      parameters[0] = inputString.substring(beginBracket + 1, endBracket + 1).toInt();
      amountOfParameters++;
    }
  }


  String methode = inputString.substring(0, beginBracket);
  if(verboseOutput)
  {
    Serial.print("Methode:   ");  Serial.println(methode);
    Serial.println();
  }

  if(methode.equalsIgnoreCase("help"))
  {
    help();
    validMethode = true;
    if(amountOfParameters == 0)
      parametersOK = true;
      Serial.println("[done]");
  }

  if(methode.equalsIgnoreCase("moveX"))
  {
    validMethode = true;
    if(amountOfParameters == 1) {
      parametersOK = true;
      moveX(parameters[0]);
    }
  }

  if(methode.equalsIgnoreCase("moveY"))
  {
    validMethode = true;
    if(amountOfParameters == 1) {
      parametersOK = true;
      moveY(parameters[0]);
    }
  }


  if(methode.equalsIgnoreCase("move"))
  {
    validMethode = true;
    if(amountOfParameters == 2) {
      parametersOK = true;
      moveXY(parameters[0], parameters[1]);
      Serial.println("[done]");
    }
  }


  if(methode.equalsIgnoreCase("resetX"))
  {
    validMethode = true;
    if(amountOfParameters == 0) {
      parametersOK = true;
      xAxis.reset();
      Serial.println("[done]");
    }
  }


  if(methode.equalsIgnoreCase("resetY"))
  {
    validMethode = true;
    if(amountOfParameters == 0) {
      parametersOK = true;
      yAxis.reset();
      Serial.println("[done]");
    }
  }


  if(methode.equalsIgnoreCase("reset"))
  {
    validMethode = true;
    if(amountOfParameters == 0) {
      Serial.println("resetting");
      parametersOK = true;
      yAxis.reset();
      xAxis.reset();
      Serial.println("[done]");
    }
  }

  /*if(methode.equalsIgnoreCase("disable")) {
    validMethode = true;
    if(amountOfParameters == 0) {
      parametersOK = true;
      yAxis.disable();
      xAxis.disable();
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("disableX")) {
    validMethode = true;
    if(amountOfParameters == 0) {
      parametersOK = true;
      xAxis.disable();
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("disableY")) {
    validMethode = true;
    if(amountOfParameters == 0) {
      parametersOK = true;
      yAxis.disable();
      Serial.println("[done]");
    }
  }


  if(methode.equalsIgnoreCase("enable")) {
    validMethode = true;
    if(amountOfParameters == 0) {
      parametersOK = true;
      if(!yAxis.isEnabled())
       // yEnableTime = millis();
      if(!xAxis.isEnabled())
        //xEnableTime = millis();
        xAxis.enable();
        yAxis.enable();
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("enableX")) {
    validMethode = true;
    if (amountOfParameters == 0) {
      parametersOK = true;
      if(!xAxis.isEnabled())
        //xEnableTime = millis();
      xAxis.enable();
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("enableY")) {
    validMethode = true;
    if(amountOfParameters == 0) {
      parametersOK = true;
      yAxis.enable();
      if(!yAxis.isEnabled())
       // yEnableTime = millis();
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("statusX")) {
    validMethode = true;
    if (amountOfParameters == 0) {
      parametersOK = true;
      Serial.println(xAxis.isEnabled());
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("statusY")) {
    validMethode = true;
    if (amountOfParameters == 0) {
      parametersOK = true;
      Serial.println(yAxis.isEnabled());
      Serial.println("[done]");
    }
  }*/

  if(methode.equalsIgnoreCase("positionX"))
  {
    validMethode = true;
    if (amountOfParameters == 0) {
      parametersOK = true;
      Serial.println(xAxis.getPosition());
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("positionY"))
  {
    validMethode = true;
    if (amountOfParameters == 0) {
      parametersOK = true;
      Serial.println(yAxis.getPosition());
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("verboseOn"))
  {
    validMethode = true;
    if(amountOfParameters == 0) {
      Serial.println("[done]");
      verboseOutput = true;
      parametersOK = true;
    }
  }

  if(methode.equalsIgnoreCase("verboseOff"))
  {
    validMethode = true;
    if(amountOfParameters == 0) {
      Serial.println("[done]");
      verboseOutput = false;
      parametersOK = true;
    }
  }

  if(methode.equalsIgnoreCase("square"))
  {
    validMethode = true;
    if(amountOfParameters == 1) {
      square(parameters[0]);
      parametersOK = true;
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("up"))
  {
    validMethode = true;
    if(amountOfParameters == 0){
      parametersOK = true;
      up();
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("down"))
  {
    validMethode = true;
    if(amountOfParameters == 0) {
      parametersOK = true;
      down();
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("ram"))
  {
    validMethode = true;
    if(amountOfParameters == 0)
      parametersOK = true;
      Serial.print("free RAM: ");
      extern int __heap_start, *__brkval;
      int v;
      Serial.println((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
      Serial.println("[done]");
  }

  if(methode.equalsIgnoreCase("diagonal"))
  {
    validMethode = true;
    if(amountOfParameters == 1) {
      Serial.println("Plotting exact diagonal ...");
      diagonal(parameters[0]);
      parametersOK = true;
      Serial.println("[done]");
    }else if(amountOfParameters == 2) {
      Serial.println("Plotting exact diagonal ...");
      diagonal(parameters[0], parameters[1]);
      parametersOK = true;
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("circle"))
  {
    validMethode = true;
    if(amountOfParameters == 1){
      circle(parameters[0]);
      parametersOK = true;
      Serial.println("[done]");
    }
  }

  if(methode.equalsIgnoreCase("ellipse"))
  {
    validMethode = true;
    if(amountOfParameters == 2){
      ellipse(parameters[0], parameters[1]);
      parametersOK = true;
      Serial.println("[done]");
    }
  }

  if(!validMethode)
  {
    Serial.print(methode);
    Serial.println("() is no valid command!");
    Serial.println("enter help() to see a list of valid commands");
  }

  if(!parametersOK && validMethode)
  {
    Serial.print("parameter mismatch! ");
    Serial.print(amountOfParameters);
    Serial.println(" parameters given.");
    Serial.println("enter help() to see a list of valid commands");
  }
  if(verboseOutput) Serial.println("[done] with command processing");
  return 0;
}

void help()
{
  Serial.println("------------------------------HELP------------------------------");
  Serial.println("Enter commands like command(argument1, ...argumentn)");
  Serial.println("Already supported commands:");
  Serial.println("help()     verboseOn()  verboseOff()  ram()");
  Serial.println("move(x,y)  moveX(x)      moveY(y)");
  Serial.println("up()       down()");
  Serial.println("reset()    resetX()      resetY() ");
  /*Serial.println("enable()   enableX()     enableY()");*/
  Serial.println(/*"statusX()  statusY()     "*/"positionX()   positionY()");
  /*Serial.println("disable()  disableX()    disableY()");*/
  Serial.println("square(l)  diagonal(l,d) circle()      ellipse()");
}

void up()
{
  zAxis.write(130);
}

void down()
{
  zAxis.write(100);
}

void moveX(int xSteps)
{
  xAxis.wakeUp();
  Serial.print("moving ");  Serial.print(xSteps); Serial.print(" steps in x-direction");
  int error = xAxis.move(xSteps);
  if(error == 0)
    Serial.println("[done]");
  else
    { Serial.print("[failed]"); Serial.println(error);  }
  xAxis.sleep();
}

void moveY(int ySteps)
{
  yAxis.wakeUp();
  Serial.print("moving ");  Serial.print(ySteps); Serial.print(" steps in y-direction ... ");
  int error = yAxis.move(ySteps);
  if(error == 0)
    Serial.println("[done]");
  else
    { Serial.print("[Error "); Serial.print(error); Serial.println("]");}
  yAxis.sleep();
}

void moveXY(int xMM, int yMM)
{
  Serial.print("moving ");  Serial.print(xMM); Serial.print(" mm in x-direction");
  Serial.print("and ");  Serial.print(yMM); Serial.println(" mm in y-direction...");
  bresenham(xMM / XmmSt, yMM / YmmSt);
}

void bresenham(int x1, int y1)
{
  wakeXYup();
  //based on code from https://de.wikipedia.org/wiki/Bresenham-Algorithmus
  int x0 = 0;
  int y0 = 0;
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = dx+dy, e2;
  Serial.println("x;y");
  while(1){
    Serial.print(x0); Serial.print(";"); Serial.println(y0);
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 > dy){
      err += dy;
      x0 += sx;
      xAxis.move(1);
    } /* e_xy+e_x > 0 */

    if (e2 < dx){
      err += dx;
      y0 += sy;
      yAxis.move(1);
    } /* e_xy+e_y < 0 */
  }
  sleepXY();
}

void square(int sideLength)
{
  wakeXYup();
  xAxis.move(sideLength);
  yAxis.move(sideLength);
  xAxis.move(-sideLength);
  yAxis.move(-sideLength);
  sleepXY();
}

void ellipse(int a, int b)  //a = width; b = height
{
  xAxis.setSpeed(500);
  yAxis.setSpeed(500);
  wakeXYup();
  Serial.print("drawing ellipse a= "); Serial.print(a); Serial.print(" b= "); Serial.println(b);
  int lastY = 0, intY = 0;
  double y, aD = a, bD = b;
  up();
  yAxis.move(b);
  down();
  //first quadrant
  Serial.println("1st quadrant");
  for(double x = 0;  x < a;  x++)
  {
    // y²= b² (1- (x² / a²))
    y = sqrt( (bD*bD) * ( 1 - ( (x*x) / (aD*aD) )));
    intY = round(y);
    setPixel(x, intY);
    xAxis.move(2);
    delay(1);
    if(x > 0)
    {
      yAxis.move(intY-lastY);
      //delay(2);
    }
    lastY = intY;
  }

  //second quadrant
  Serial.println("2nd quadrant");
  for(double x = a;  x > 0;  x--)
  {
    // y²= b² (1- (x² / a²))
    y = -sqrt( (bD*bD) * ( 1 - ( (x*x) / (aD*aD) )));
    intY = round(y);
    setPixel(x, round(y));

    xAxis.move(1);
    delay(1);
    if(x > 0)
    {
        yAxis.move(intY-lastY);
    }
    lastY = intY;
  }
  //third quadrant
  Serial.println("3rd quadrant");
  for(double x = 0;  x > -a;  x--)
  {
    // y²= b² (1- (x² / a²))
    y = -sqrt( (bD*bD) * ( 1 - ( (x*x) / (aD*aD) )));
    intY = round(y);
    setPixel(x, round(y));

    xAxis.move(-1);
    delay(1);
    if(x > 0)
    {
        yAxis.move(intY-lastY);
    }
    lastY = intY;
  }
  //forth quadrant
  Serial.println("4th quadrant");
  for(double x = -a;  x < 0; x++ )
  {
    // y²= b² (1- (x² / a²))
    y = sqrt( (bD*bD) * ( 1 - ( (x*x) / (aD*aD) )));
    intY = round(y);
    setPixel(x, round(y));

    xAxis.move(-1);
    delay(1);
    if(x > 0)
    {
        yAxis.move(intY-lastY);
    }
    lastY = intY;
  }
  sleepXY();
}

void circle(int radius)
{
  Serial.println(radius);
  Serial.println(radius / XmmSt);
  Serial.println(radius / YmmSt);
  ellipse(radius / XmmSt, radius / YmmSt);
}

void setPixel(int x, int y)
{
  Serial.print(x); Serial.print("\t"); Serial.println(y);
  /*for(int i = 0; i < y; i++)
    Serial.print(" ");
  Serial.println(".");*/
}

void wakeXYup()
{
  xAxis.wakeUp();
  yAxis.wakeUp();
}

void sleepXY()
{
    xAxis.sleep();
    yAxis.sleep();
}


