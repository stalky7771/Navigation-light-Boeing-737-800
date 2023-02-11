#include <Thread.h>  // подключение библиотеки ArduinoThread

const int ANTICOLLISION_1 = 10; // ШИМ
const int ANTICOLLISION_2 = 11; // ШИМ
const int STROBE_1 = 7;
const int STROBE_2 = 8;

const int dimmerPin = 11;  // переменная с номером пина пьезоэлемента
const int ledPin = 13;  // переменная с номером пина светодиода
const int togglePin = 4;
int brightness = 0;    // уставливаем начально значение яркости
int fadeAmount = 50;    // шаг приращения/убывания яркости

int currentAntiCollisionPin = ANTICOLLISION_1;
int currentStrobePin = STROBE_1;

Thread antiCollisionThread = Thread();
Thread strobeThread = Thread();

void setup()
{
  pinMode(ANTICOLLISION_1, OUTPUT);
  pinMode(ANTICOLLISION_2, OUTPUT);
  pinMode(STROBE_1, OUTPUT);
  pinMode(STROBE_2, OUTPUT);
  
  pinMode(dimmerPin, OUTPUT); // объявляем пин 3 как выход.
  pinMode(ledPin, OUTPUT);   // объявляем пин 13 как выход.
  pinMode(togglePin, OUTPUT);

  antiCollisionThread.onRun(antiCollisionTimerCallback);
  antiCollisionThread.setInterval(25);

  strobeThread.onRun(strobeTimerCallback);
  strobeThread.setInterval(1000);
}

void loop()
{
    if (antiCollisionThread.shouldRun())
        antiCollisionThread.run();

    if (strobeThread.shouldRun())
        strobeThread.run();    
}

void antiCollisionTimerCallback()
{ 
    brightness = brightness + fadeAmount; 

  if (brightness == 0 || brightness == 250)
  {
    fadeAmount = -fadeAmount ;
  }   

  analogWrite(currentAntiCollisionPin, brightness);  

  if (brightness == 0 || brightness == 250)
  {
    delay(500);    
  }    
  else
  {
    delay(10);
  }    

  if (brightness == 0)
  {
    toggleAntiCollisionPin();
  }
}

void strobeTimerCallback()
{
  digitalWrite(currentStrobePin, 1);
  delay(50);
  digitalWrite(currentStrobePin, 0);  

  if(currentStrobePin == STROBE_1)
  {
    delay(50);
    digitalWrite(currentStrobePin, 1);
    delay(50);
    digitalWrite(currentStrobePin, 0);
  }

  toggleStrobePin();
}

void toggleAntiCollisionPin()
{
  if(currentAntiCollisionPin == ANTICOLLISION_1)
    currentAntiCollisionPin = ANTICOLLISION_2;
  else
    currentAntiCollisionPin = ANTICOLLISION_1;
}

void toggleStrobePin()
{
  if(currentStrobePin == STROBE_1)
    currentStrobePin = STROBE_2;
  else
    currentStrobePin = STROBE_1;
}
