#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;


const float R_aire = 287.058; 

void setup() {
  Serial.begin(9600);
  

  if (!bmp.begin()) {
    Serial.println("¡No se pudo encontrar un sensor BMP180 válido! Revisa las conexiones.");
    while (1) {} 
  }
  Serial.println("Sensor BMP180 inicializado correctamente.");
}

void loop() {

  float temperaturaC = bmp.readTemperature();
  float temperaturaK = temperaturaC + 273.15;

  float presionPa = bmp.readPressure(); 

  float densidadAire = presionPa / (R_aire * temperaturaK);

  //Serial.print("Temperatura: ");
  //Serial.print(temperaturaC);
  //Serial.print(" °C | Presión: ");
 // Serial.print(presionPa / 100.0); 
 // Serial.print(" hPa | ");
  
  Serial.print("Temperatura:  ");
  Serial.print(temperaturaC , 4); 
  Serial.println(" °C");

  Serial.print("Temperatura lm35:  ");
  Serial.print(temperaturaC + 2.1, 4); 
  Serial.println(" °C");
  delay(2000); 
}