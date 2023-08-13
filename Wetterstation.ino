#define SPI_CS 4
#define SENSORPIN 2

#include "SPI.h"
#include"SD.h"
#include"DHT.h"
#include"DS3231.h"

DHT dht(SENSORPIN, DHT11);
DS3231 Uhr;

float Temp;
float Feuchte;
boolean Dummy = false;


void setup() {
  dht.begin();
  Wire.begin();

  Serial.begin(9600);
  Serial.println("SD-Karte initialisieren . . .");

  if(!SD.begin(SPI_CS))
  {
    Serial.println("Karte nicht lesbar!");
    while(1);
  }
  Serial.println("erfolgreich.");

  if (SD.exists("temp_log.txt")) {
    Serial.println("temp_log.txt existiert.");
  } else {
    Serial.println("temp_log.txt existiert nicht");
  }
}

void loop() {

  Temp = dht.readTemperature();
  Feuchte = dht.readHumidity();

  String Datensatz = "# " +
    String(Uhr.getDate(),2) + "." +
    String(Uhr.getMonth(Dummy)) + "." +
    Uhr.getYear() + ", " +
    Uhr.getHour(Dummy, Dummy) + ":";    

  if(Uhr.getMinute() < 10)
    Datensatz = Datensatz + "0";

  Datensatz = Datensatz + Uhr.getMinute() + ":";

  if(Uhr.getSecond() < 10)
    Datensatz = Datensatz + "0";

  Datensatz = Datensatz + Uhr.getSecond() +
    " # Temperatur: " + String(Temp) +
    " # Feuchte: " + String(Feuchte) + " #";

  Serial.println(Datensatz);

  File Zieldatei = SD.open("temp_log.txt", FILE_WRITE);
  if(Zieldatei)
  {
    Zieldatei.println(Datensatz);
    Zieldatei.close();    
  }
  else{
    Serial.println("Datensatz konnte nicht geschrieben werden!");
  }

  delay(15000);
}
