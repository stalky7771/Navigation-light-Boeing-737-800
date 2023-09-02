#include <Thread.h>     // подключение библиотеки ArduinoThread
#include <DFMiniMp3.h>  // https://electronoobs.com/eng_arduino_DFPlayer_Mini_Mp3_by_Makuna.php
#include "dimmer.h"
#include "Mp3Notify.h"

class Mp3Notify;

const int ANTICOLLISION_1 = 11; // ШИМ
const int ANTICOLLISION_2 = 10; // ШИМ 
const int POWER_BLOCK_PIN = 9; // ШИМ
const int STROBE_1 = 8;
const int STROBE_2 = 7;
const int POWER_OFF_PIN = 13;
const int WAKEUP_PIN = 3;
const int TEST_PIN = 5;

const int dimmerPin = 11;  // переменная с номером пина пьезоэлемента
const int ledPin = 13;  // переменная с номером пина светодиода
const int togglePin = 4;

int currentAntiCollisionPin = ANTICOLLISION_1;
int currentStrobePin = STROBE_1;
bool isPowerBlockEnabled = false;

Thread antiCollisionThread = Thread();
Thread strobeThread = Thread();
Thread mp3Thread = Thread();
Thread powerBlockOffThread = Thread();

void toggleAntiCollisionPin()
{
  if(currentAntiCollisionPin == ANTICOLLISION_1)
    currentAntiCollisionPin = ANTICOLLISION_2;
  else
    currentAntiCollisionPin = ANTICOLLISION_1;
}

void dimmerAntiCollisionOnMin() { toggleAntiCollisionPin(); }
void dimmerAntiCollisionOnMax() {}
void dimmerAntiCollisionOnTick(int br) { /*analogWrite(currentAntiCollisionPin, br);*/ }

// POWER DIMMER ********************************************************************
void dimmerPowerBlockOnMin()
{  
  isPowerBlockEnabled = false;
  digitalWrite(POWER_OFF_PIN, 0);
}

void dimmerPowerBlockOnMax()
{
  isPowerBlockEnabled = false;
}

void dimmerPowerBlockOnTick(int br)
{
  analogWrite(POWER_BLOCK_PIN, br);
}
//~POWER DIMMER ********************************************************************

Dimmer dimmerAntiCollision = Dimmer(dimmerAntiCollisionOnMin, dimmerAntiCollisionOnMax, dimmerAntiCollisionOnTick, 25);
Dimmer dimmerPowerBlock = Dimmer(dimmerPowerBlockOnMin, dimmerPowerBlockOnMax, dimmerPowerBlockOnTick, 5);

DFMiniMp3<HardwareSerial, Mp3Notify> mp3(Serial);   //Create the UART connection to the module

void setup()
{
  pinMode(WAKEUP_PIN, INPUT_PULLUP);

  Serial.begin(9600); 
  mp3.begin();

  pinMode(ANTICOLLISION_1, OUTPUT);
  pinMode(ANTICOLLISION_2, OUTPUT);
  pinMode(STROBE_1, OUTPUT);
  pinMode(STROBE_2, OUTPUT);

  //pinMode(TEST_PIN, OUTPUT);
  //digitalWrite(TEST_PIN, 1);

  pinMode(POWER_OFF_PIN, OUTPUT);
  digitalWrite(POWER_OFF_PIN, 1);
  
  pinMode(dimmerPin, OUTPUT); // объявляем пин 3 как выход.
  pinMode(ledPin, OUTPUT);   // объявляем пин 13 как выход.
  pinMode(togglePin, OUTPUT);

  antiCollisionThread.onRun(antiCollisionTimerCallback);
  antiCollisionThread.setInterval(25);

  strobeThread.onRun(strobeTimerCallback);
  strobeThread.setInterval(1000);

  mp3Thread.onRun(mp3TimerCallback);
  mp3Thread.setInterval(1000);
  
  powerBlockOffThread.onRun(powerBlockOffCallback);
  powerBlockOffThread.setInterval(45000);
}

/*void wakeup()
{
  power.wakeUp();
  powerBlockOffThread.onRun(powerBlockOffCallback);
  powerBlockOffThread.setInterval(10000);
}*/

void loop()
{
    if(antiCollisionThread.shouldRun())
      antiCollisionThread.run();

    if(powerBlockOffThread.shouldRun())
      powerBlockOffThread.run();

    if(strobeThread.shouldRun())
      strobeThread.run();   

    if(mp3Thread.shouldRun()) 
      mp3Thread.run();
}

void powerBlockOffCallback()
{
  //powerBlockOffThread.enabled = false;
  isPowerBlockEnabled = true;
}

void antiCollisionTimerCallback()
{ 
  dimmerAntiCollision.Tick();
  
  if (isPowerBlockEnabled)
    dimmerPowerBlock.Tick();
}

void strobeTimerCallback()
{
  digitalWrite(currentStrobePin, 1);
  digitalWrite(ANTICOLLISION_1, 1);
  digitalWrite(ANTICOLLISION_2, 1);
  
  delay(50);
  digitalWrite(currentStrobePin, 0);  

  delay(50);
  digitalWrite(ANTICOLLISION_1, 0);
  digitalWrite(ANTICOLLISION_2, 0);

  if (currentStrobePin == STROBE_1)
  {
    delay(50);
    digitalWrite(currentStrobePin, 1);
    delay(50);
    digitalWrite(currentStrobePin, 0);
  }

  toggleStrobePin();
}

void mp3TimerCallback()
{
  isPowerBlockEnabled = true;
  
  mp3Thread.enabled = false;
  randomSeed(analogRead(0));
  int trackNumber = random(1, 11);
  mp3.playMp3FolderTrack(trackNumber);  // Play audio track 0001
  delay(2000);
  mp3.setVolume(28); 
}

void toggleStrobePin()
{
  if (currentStrobePin == STROBE_1)
    currentStrobePin = STROBE_2;
  else
    currentStrobePin = STROBE_1;
}