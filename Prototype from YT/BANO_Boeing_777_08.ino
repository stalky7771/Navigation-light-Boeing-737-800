// Прошивка для имитации освещения салона и бортовых авиа-навигационных огней модели самолета Boeing-777
//автор: lukys30

#include <IRremote.h>
int RECV_PIN = 2;               //Сигнал от ИК приемника приходит на пин 2
IRrecv irrecv(RECV_PIN);
decode_results results;

//Объявляем все источники света

bool Landing_Lights = false;                    // кнопка 7
bool Taxi_Lights = false;                       // кнопка 8
bool Runway_Lights = false;                     // кнопка 9
bool Position_Lights = true;                    // кнопка *
bool Wing_Illumination_Lights = false;           // кнопка 0
bool Logo_Lights = true;                        // кнопка #
bool Saloon_static_mode = false;                      // кнопка ^
bool Saloon_spectrum_mode = true;               // кнопка ^ длительно
bool Beacon_Lights = true;                      // кнопка <
bool Strobe_Lights = true;                      // кнопка >
byte R_Saloon_static = 30;
byte G_Saloon_static = 30;
byte B_Saloon_static = 30;
byte PrevKey =0;                                // Номер последней нажаной кнопки на пульте ДУ (используется для быстрого увеличения значений при зажатии)
byte Max_RGB_Value = 255;
byte StepFade = 15;
long int Blink_start_time;
long int Blink_time;
long int fade_goTime;
int fade_break = 40;
int sensorValue;
  unsigned int rgbColour[3] = {255,0,0}; //Start on red
   int incColour = 1;
   int decColour = 0;
   int i = -1;
byte R_Saloon, G_Saloon, B_Saloon;

   
void setup()
{

    fade_goTime = millis(); 
    
    pinMode(9, OUTPUT);                   // ШИМ салона красный   Red
    pinMode(4, OUTPUT);                   // Landing_Lights
    pinMode(5, OUTPUT);                   // ШИМ салона зеленый   Green
    pinMode(6, OUTPUT);                   // ШИМ салона синий     Blue
    pinMode(7, OUTPUT);                   // Taxi_Lights
    pinMode(8, OUTPUT);                   // Position_Lights
                                          // A3 аналоговый вход для фоторезистора
    pinMode(A2, OUTPUT);                  // Runway_Lights
    pinMode(A1, OUTPUT);                  // Logo_Lights
    pinMode(A0, OUTPUT);                  // Beacon_Lights
    pinMode(11, OUTPUT);                  // Strobe_Lights
    pinMode(13, OUTPUT);                  // Wing_Illumination_Lights
                                        
    fade_goTime = millis();               
    int fade_break = 100;                    // Скорость изменения спектра освещения салона

    Blink_start_time = millis();
    Blink_time = 0;
    
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
   //---------------------- Считываем значение с ИК приемника -----------------------
    if (irrecv.decode(&results)) {
 //    Serial.println(results.value, HEX);         // отладка

 
  switch ( results.value ) {
    case 0xFD00FF:{                               // Если с пульта нажата кнопка "1" - увеличиваем красный цвет салона
        PrevKey = 1;
          if (R_Saloon_static != Max_RGB_Value) R_Saloon_static += StepFade;
 
        } break;
    case 0xFD807F: {                              // Если с пульта нажата кнопка "2" - увеличиваем зеленый цвет салона
        PrevKey = 2;
          if (G_Saloon_static != Max_RGB_Value) G_Saloon_static += StepFade;
         } break;
    case 0xFD40BF: {                                 // Если с пульта нажата кнопка "3" - увеличиваем синий цвет салона
        PrevKey = 3;
          if (B_Saloon_static != Max_RGB_Value) B_Saloon_static += StepFade;
         } break;
    case 0xFD20DF:{                                 // Если с пульта нажата кнопка "4" - уменьшаем красный цвет салона
           PrevKey = 4;
           if (R_Saloon_static != 0) R_Saloon_static -= StepFade;
        } break;
    case 0xFDA05F:{                                 // Если с пульта нажата кнопка "5" - уменьшаем зеленый цвет салона
           PrevKey = 5;
           if (G_Saloon_static != 0) G_Saloon_static -= StepFade;
        } break;
    case 0xFD609F:{                                 // Если с пульта нажата кнопка "6" - уменьшаем синий цвет салона
           PrevKey = 6;
           if (B_Saloon_static != 0) B_Saloon_static -= StepFade;
         } break;
    case 0xFD10EF:  Landing_Lights=!Landing_Lights;                     break;                // Если с пульта нажата кнопка "7", то включаем/отключаем Landing_Lights
    case 0xFD906F:  Taxi_Lights=!Taxi_Lights;                           break;                // Если с пульта нажата кнопка "8", то включаем/отключаем Taxi_Lights
    case 0xFD50AF:  Runway_Lights=!Runway_Lights;                       break;                // Если с пульта нажата кнопка "9", то включаем/отключаем Runway_Lights
    case 0xFD30CF:  Position_Lights=!Position_Lights;                   break;                // Если с пульта нажата кнопка "*", то включаем/отключаем Position_Lights
    case 0xFDB04F:  Wing_Illumination_Lights=!Wing_Illumination_Lights; break;                // Если с пульта нажата кнопка "0", то включаем/отключаем Wing_Illumination_Lights
    case 0xFD708F:  Logo_Lights=!Logo_Lights;                           break;                // Если с пульта нажата кнопка "#", то включаем/отключаем Logo_Lights
    case 0xFD8877:  {
                       Saloon_static_mode=!Saloon_static_mode;                    // Если с пульта нажата кнопка "Вверх", то включаем/отключаем статичное освещение салона
                       if (Saloon_static_mode) Saloon_spectrum_mode=false;
                    } break; 
    case 0xFD9867:  {
                          Saloon_spectrum_mode=!Saloon_spectrum_mode;                 // Если с пульта нажата кнопка "Вниз", то включаем/отключаем динамичное освещение салона
                       if (Saloon_spectrum_mode) Saloon_static_mode=false;
                    } break; 

    case 0xFD28D7:  Beacon_Lights = !Beacon_Lights;                     break;                // Если с пульта нажата кнопка "Влево", то включаем/отключаем Beacon_Lights
    case 0xFD6897:  Strobe_Lights = !Strobe_Lights;                     break;                // Если с пульта нажата кнопка "Вправо", то включаем/отключаем Strobe_Lights
    case 0xFDA857:  {                                                                         // Если с пульта нажата кнопка "ОК"  то включаем/отключаем все огни
                   
                    };                                                  break;
    case 0xFFFFFFFF:  {                                                                      // Если с пульта любая кнопка не отжимается
           switch ( PrevKey )  {
            case 1: if (R_Saloon_static != Max_RGB_Value) R_Saloon_static += StepFade;  break;
            case 2: if (G_Saloon_static != Max_RGB_Value) G_Saloon_static += StepFade;  break;
            case 3: if (B_Saloon_static != Max_RGB_Value) B_Saloon_static += StepFade;  break;
            case 4: if (R_Saloon_static != 0) R_Saloon_static -= StepFade;              break;
            case 5: if (G_Saloon_static != 0) G_Saloon_static -= StepFade;              break;
            case 6: if (B_Saloon_static != 0) B_Saloon_static -= StepFade;              break;
   //         case 7: Saloon_spectrum_mode =! Saloon_spectrum_mode;                       break;
                      };
                      };                                                  break;                     // конец обработки неотжатой кнопки
     
    
 
    }  // Конец цикла опроса пульта  (обработаны все 17 вариантов нажатий на кнопки пульта, а так же неотжатие при увеличении/уменьшении RGB подсветки салона кнопками 1-6)  
    
    irrecv.resume(); // Ждем следующего сигнала с ИК датчика
    
    
    }

    //------------- освещаем салон и огни в соответствии с выбранным режимом огней ---------------------

    digitalWrite(4, Landing_Lights); 
    digitalWrite(7, Taxi_Lights);  
    digitalWrite(A2,Runway_Lights);   
    digitalWrite(8, Position_Lights);  
    digitalWrite(A1,Logo_Lights);
    digitalWrite(13,Wing_Illumination_Lights);
                                            // Мигание стробой и беконом
      if (Blink_time > 4000) Blink_start_time = millis();
      Blink_time = millis() - Blink_start_time;
      if ((Blink_time >0) & (Blink_time <500))      digitalWrite(A0,Beacon_Lights); else digitalWrite(A0,false) ;        // светим беконом первые 500 мс из 4000 по циклу если он включен                              
      if ((Blink_time >2000) & (Blink_time <2070))  digitalWrite(11,Strobe_Lights); else digitalWrite(11,false) ;        // первая вспышка стробы 
      if ((Blink_time >2350) & (Blink_time <2400))  digitalWrite(11,Strobe_Lights); else digitalWrite(11,false) ;        // вторая вспышка стробы  (на самом деле второй вспышки у B-777 нет, но зато есть у B-737)

if((  millis() >= fade_goTime) & Saloon_spectrum_mode )
  {
     // cross-fade the two colours.
      i++;
      if(i > 254) {
        i=0;
        decColour ++;
        if(decColour >2) decColour = 0;      
        if (decColour == 2)  incColour = 0;
        else incColour = decColour +1;
      }
        rgbColour[decColour] -= 1;
        rgbColour[incColour] += 1;
       
        R_Saloon=rgbColour[0];
        G_Saloon=rgbColour[1];
        B_Saloon=rgbColour[2];      
      
       fade_goTime = millis() + fade_break;     
  }

   if (Saloon_static_mode)
   {
    analogWrite(9, R_Saloon_static);
    analogWrite(5, G_Saloon_static);
    analogWrite(6, B_Saloon_static);
   }

   if (Saloon_spectrum_mode)
   {
      
    analogWrite(9, R_Saloon);
    analogWrite(5, G_Saloon);
    analogWrite(6, B_Saloon);
   } 
 if ((!Saloon_static_mode)&(!Saloon_spectrum_mode))
   {
      
    analogWrite(9, 0);
    analogWrite(5, 0);
    analogWrite(6, 0);
   } 
    
 sensorValue = analogRead(A3);
 Serial.print(" Sensor= " ); Serial.println(sensorValue, DEC);
// отладка    
/*    Serial.print(" R= " ); Serial.print(R_Saloon, DEC);
    Serial.print(" G= " ); Serial.print(G_Saloon, DEC);
    Serial.print(" B= " ); Serial.println(B_Saloon, DEC);
    Serial.print(" Rs= " ); Serial.print(R_Saloon_static, DEC);
    Serial.print(" Gs= " ); Serial.print(G_Saloon_static, DEC);
    Serial.print(" Bs= " ); Serial.println(B_Saloon_static, DEC);
*/
}
