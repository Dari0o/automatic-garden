
//more Information at: https://www.aeq-web.com/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include "RTClib.h"
#include "DHT.h"

const int chipSelect = 10; //10 is default by shield, but normally on Pin 4
int interval = 30;  //Log to SD Card every 5 seconds

long timer;
String timestring;
String mvalue;

RTC_DS1307 rtc;

DHT dht(2, DHT11);

void setup() {
  dht.begin();

  pinMode(5, INPUT);
  pinMode(6, INPUT);
  
  Serial.begin(9600);
  delay(3000);
  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card error");
    return;
  }
  Serial.println("card initialized");
  if (! rtc.begin()) {
    Serial.println("No RTC found");
  } else {
    Serial.println("RTC clock found");
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is not configured");
  }
  rtc.adjust(DateTime(2021, 11, 2, 17, 18, 0));
}

void loop() {
  
  if ((timer + interval * 100) < millis()) {
    timer = millis();
    get_logvalue(); //Get your value
    write_data(); //Write value and Time to SD
  }
}

void get_logvalue() {
  
  mvalue = "My Value"; //mvalue is your log parameter eg. Temperature
}

void write_data() { //Write to SD card

  float Temp = dht.readTemperature();
  float Feuchte = dht.readHumidity();

  if(Feuchte < 60){
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
  }
  
  DateTime now = rtc.now();
  //Serial.println(timestring);
  //String timestring = now.year() + "-" + now.month() + "-" + now.day() + " " + now.hour() + ":" + now.minute() + ":" now.second();
  
  //String dataString = " # Temperatur in C: " + Temp + ", Feuchte in %: " + Feuchte;
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(now.year(), DEC);
    dataFile.print(" ");
    dataFile.print(now.month(), DEC);
    dataFile.print("-");
    dataFile.print(now.day(), DEC);
    dataFile.print(" ");
    dataFile.print(now.hour(), DEC);
    dataFile.print(":");
    dataFile.print(now.minute(), DEC);
    dataFile.print(":");
    dataFile.print(now.second(), DEC);
    dataFile.print(" ");
    dataFile.print(" # Temperatur in C: ");
    dataFile.print(Temp);
    dataFile.print(" # Luftfeuchtigkeit in %: ");
    dataFile.print(Feuchte);
    dataFile.println();
    dataFile.close(); 


    Serial.print(now.year(), DEC);
    Serial.print("-");
    Serial.print(now.month(), DEC);
    Serial.print("-");
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.print(now.second(), DEC);
    Serial.print(" ");
    Serial.print(" # Temperatur in C: ");
    Serial.print(Temp);
    Serial.print(" # Luftfeuchtigkeit in %: ");
    Serial.print(Feuchte);
    Serial.println();
  }
  else {
    Serial.println("error writing datalog.txt");
  }
}
