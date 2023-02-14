//#include <SoftwareSerial.h>   //If we use other pins than D0 and D1 for TX and RX for the music player
#include <DFMiniMp3.h>          //Downlaod here: https://electronoobs.com/eng_arduino_DFPlayer_Mini_Mp3_by_Makuna.php

int sw1 = 3;
int sw2 = 4;
int sw3 = 5;

class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }

  static void OnPlayFinished(uint16_t globalTrack)
  {
    Serial.println();
    Serial.print("Play finished for #");
    Serial.println(globalTrack);   
  }

  static void OnCardOnline(uint16_t code)
  {
    Serial.println();
    Serial.print("Card online ");
    Serial.println(code);     
  }

  static void OnCardInserted(uint16_t code)
  {
    Serial.println();
    Serial.print("Card inserted ");
    Serial.println(code); 
  }

  static void OnCardRemoved(uint16_t code)
  {
    Serial.println();
    Serial.print("Card removed ");
    Serial.println(code);  
  }
};

DFMiniMp3<HardwareSerial, Mp3Notify> mp3(Serial);   //Create the UART connection to the module

void setup() 
{
  //3 push buttons with pullups
  pinMode(sw1,INPUT_PULLUP);                        //Define each button as input with pullup
  pinMode(sw2,INPUT_PULLUP);
  pinMode(sw3,INPUT_PULLUP);
  
  Serial.begin(9600); 
  mp3.begin();                                      //Start communication with the DFplayer module
  //uint16_t volume = mp3.getVolume();                //Get actual volume
  //delay(100);
  mp3.setVolume(15);                                //Set new volum (max is 30)
  waitMilliseconds(1000);

  randomSeed(analogRead(0));
  int trackNumber = random(0, 4);
  mp3.playMp3FolderTrack(trackNumber);  // Play audio track 0001
  //uint16_t count = mp3.getTotalTrackCount();        //Get the total tracks on the SD card in case we want to sue this later...
}


//just a fucntion that we use to create delays in "ms" 
//without using the delay() function
void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}

void loop() 
{

  if(!digitalRead(sw1))
  {    
      mp3.playMp3FolderTrack(1);  // Play audio track 0001
      waitMilliseconds(1000);     // 1s of delay
  }
  
  if(!digitalRead(sw2))
  {    
      mp3.playMp3FolderTrack(2);  // Play audio track 0002
      waitMilliseconds(1000);     // 1s of delay
  }
  
  if(!digitalRead(sw3))
  {    
      mp3.playMp3FolderTrack(6);  // Play audio track 0003
      waitMilliseconds(1000);     // 1s of delay; 
  }
}//end of void