#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>
#include <NewPing.h>
LiquidCrystal lcd (8,7,5,4,3,6);
NewPing sonar(11, 10);
Servo serv;

int a=0;
int b=0;
int c=0;
int d=0;
int SI=1;
int NO=1;

double tinicio;
int lectura;
float volt;
int temp;
int tmax=0;
int tmin=255;

int distancia ;
int aper=EEPROM.read(2);

int TAPA=0;

#if defined(ARDUINO_ARCH_SAMD)

   #define Serial SerialUSB
#endif

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup() {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  lcd.begin(16,2);
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2018, 5, 20, 12, 52, 0));
  }

  lcd.begin(16,2);
  pinMode(A1, INPUT_PULLUP);
  pinMode (12,INPUT_PULLUP);
  pinMode (2,INPUT_PULLUP);
  attachInterrupt ( 0,opcion,FALLING); 
  tinicio=millis();
  Serial.begin(9600);
  serv.attach(9);
  serv.write(0);
  delay(200);
  serv.write(90);
}

void loop() {
  DateTime now = rtc.now();
  switch (a){
    case 0 :
    
      if (d == 0){
      Serial.println ("caso 0");
      lcd.clear();
      d=1;
      if (TAPA==1){
              Serial.println("cerrando");
              serv.write(0);
              delay(250);
              serv.write (90);
              TAPA=0;     
            }
      }

      distancia = sonar.ping_cm();
   
      if ( distancia <= 20 && distancia >1){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("EN PROCESO");
        Serial.println("en proceso");
        serv.write (180);
        delay(200);
        serv.write (90);
        delay(3000);
        serv.write(0);
        delay(250);
        serv.write (90);
        aper++;
        if (aper>EEPROM.read(2)){
          EEPROM.write(2,aper);
          
        }
        
      d=0;
      }delay(200);
     
      Serial.println(aper);
      lcd.setCursor(0,0);
      lectura=analogRead(A0);
      volt=(lectura/1024.0)*5.0;
      temp = (volt) * 100; 
      lcd.setCursor(11,1);
      lcd.print(temp);
      lcd.print("\337"); 
      lcd.print ("C");
  
      if (temp<=tmin) tmin=temp;
      if (temp>=tmax) tmax=temp;
 
      if (tmin<(EEPROM.read(0))){
         EEPROM.write(0,tmin);
      }
      if (tmax>(EEPROM.read(1))){
        EEPROM.write(1,tmax);
     }
  
     

    lcd.setCursor(0,0);
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
    lcd.setCursor(11,0);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
    lcd.setCursor(0,1);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    lcd.print("  ");

    
      
     break;
    case 1 : 
    
      if (d ==0){
      Serial.println ("caso 1");
      lcd.clear();
      d=1;
      }
      
      if (b==0){
      Serial.println("temperaturas");
      lcd.setCursor(2,0);
      lcd.print ("TEMPERATURAS");
      }
         Serial.println("Le has dado a si");
         SI=digitalRead (A1);
        if (SI==0){
          b=1;
          c=0;
          lcd.clear();
          Serial.println("SI");
          lcd.print("Max:");
          lcd.print(EEPROM.read(1));
          lcd.print("\337"); 
          lcd.print ("C");
          lcd.setCursor(10,0);
          lcd.print("RESET?");
          lcd.setCursor (0,1);
          lcd.print ("Min:");
          lcd.print(EEPROM.read(0));
          lcd.print("\337"); 
          lcd.print ("C");
          lcd.setCursor(10,1);
          lcd.print ("Si No ");
          delay (1000);
          while (c==0){
          Serial.println ("eleccion");
          SI=digitalRead (A1);
            if (SI==0){
            lcd.clear();
            lcd.print("  RESETEANDO");
            EEPROM.write(0,255);
            EEPROM.write(1,0);
            tmin=255;
            tmax=0;  
            delay(500);
            lcd.print(".");
            delay(500);
            lcd.print(".");
            delay(500);
            lcd.print(".");
            delay(500);       
            lcd.clear();
            a=0;
            b=0;
            c=1;
            d=0;
          }
          NO=digitalRead(12);
            if (NO==0){
              Serial.print("NO");
              lcd.clear();
              a=0;
              b=0;
              c=1;
              d=0;
            }
          }
  
        }
      
     break;
    case 2 :
      if (d ==0){
      Serial.println ("caso 2");
      lcd.clear();
      d=1;
      }
      if (b==0){
      Serial.println("aperturas");
      lcd.setCursor(0,0);
      lcd.print ("N");
      lcd.print("\337"); 
      lcd.print(" DE APERTURAS");
      }
      Serial.println("Le has dado a si");
         SI=digitalRead (A1);
        if (SI==0){
          b=1;
          c=0;
          lcd.clear();
          Serial.println("SI");
          lcd.print("APERTURAS:");
          lcd.setCursor(10,0);
          lcd.print("RESET?");
          lcd.setCursor (1,1);
          lcd.print (EEPROM.read(2));
          lcd.setCursor(10,1);
          lcd.print ("Si No ");
          delay(1000);
          while (c==0){
          Serial.println ("eleccion");
          SI=digitalRead (A1);
            if (SI==0){
            lcd.clear();
            lcd.print("  RESETEANDO");
            EEPROM.write(2,0);
            aper=0;
            delay(500);
            lcd.print(".");
            delay(500);
            lcd.print(".");
            delay(500);
            lcd.print(".");
            delay(500);  
            lcd.clear();
            a=0;
            b=0;
            c=1;
            d=0;
          }
          NO=digitalRead(12);
            if (NO==0){
              Serial.print("NO");
              lcd.clear();
              a=0;
              b=0;
              c=1;
              d=0;
            }
          }
          
          
      
        }
     break;
    case 3 :
    if (d ==0){
      Serial.println ("caso 3");
      lcd.clear();
      d=1;
      }
       if (b==0){
        lcd.setCursor(5,0);
        lcd.print ("MANUAL");
         SI=digitalRead (A1);
        if (SI==0){
          b=1;
          c=0;
          lcd.clear();
          Serial.println("SI");
          lcd.setCursor(6,0);
          lcd.print("TAPA");
          lcd.setCursor(1,1);
          lcd.print("ABRIR");
          lcd.setCursor(9,1);
          lcd.print("CERRAR");
          delay (1000);
          while (c==0){
          Serial.println ("eleccion");
          SI=digitalRead (A1);
            if (SI==0 && TAPA==0){
            lcd.clear();
            Serial.println("abriendo");
            serv.write (180);
            delay(200);
            serv.write (90);
            lcd.clear();
            
            b=0;
            c=1;
            d=0;
            TAPA=1;
          }
          NO=digitalRead(12);
            if (NO==0 && TAPA==0){
              Serial.print("NO");
              lcd.clear();
              a=0;
              b=0;
              c=1;
              d=0;
            }
            if (NO==0 && TAPA == 1){
              Serial.print("NO");
              lcd.clear();
              Serial.println("cerrando");
              serv.write(0);
              delay(250);
              serv.write (90);
              a=0;
              b=0;
              c=1;
              d=0;
              TAPA=0;
              
            }
          }
        
       }

     break;
  }
 
}
}
void opcion(){
    
   if ( millis( )>tinicio+250)
          {   
            a++ ;
            b=0;
            c=1;
            d=0;
            NO=0;
            
            
            tinicio=millis();
          }
    if(a==4)a=0;
  
}



