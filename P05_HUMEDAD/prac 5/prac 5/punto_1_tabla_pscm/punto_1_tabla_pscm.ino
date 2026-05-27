#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP085.h>

// Crear los objetos para los sensores I2C
Adafruit_AHTX0 aht;
Adafruit_BMP085 bmp;

// Definición del pin analógico para el LM35
const int pinLM35 = A0;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10); // Esperar a que se abra el monitor serie

  Serial.println(F("=========================================================================="));
  Serial.println(F("                    Inicializando Sistema de Sensores                     "));
  Serial.println(F("=========================================================================="));

  // 1. Inicializar Sensor AHT20
  if (!aht.begin()) {
    Serial.println(F("¡Error! No se encontró el sensor AHT20."));
    while (1) delay(10);
  }
  Serial.println(F("-> AHT20 detectado correctamente."));

  // 2. Inicializar Sensor BMP180
  if (!bmp.begin()) {
    Serial.println(F("¡Error! No se encontró el sensor BMP180."));
    while (1) delay(10);
  }
  Serial.println(F("-> BMP180 detectado correctamente."));

  Serial.println(F("-> LM35 (Analógico) listo en pin A0."));
  Serial.println(F("=========================================================================="));
  
  // Encabezado de las 4 columnas
  Serial.println(F("Temp AHT20\t|\tTemp BMP180\t|\tTemp LM35\t|\tHumedad AHT20"));
  Serial.println(F("=========================================================================="));
}

void loop() {
  // --- Lectura del AHT20 (Temperatura y Humedad) ---
  sensors_event_t humidity, tempAHT;
  aht.getEvent(&humidity, &tempAHT);
  float t_aht = tempAHT.temperature;
  float h_aht = humidity.relative_humidity;

  // --- Lectura del BMP180 ---
  float t_bmp = bmp.readTemperature();

  // --- Lectura del LM35 ---
  int lecturaADC = analogRead(pinLM35);
  float t_lm35 = lecturaADC * (500.0 / 1023.0);

  // --- Impresión de datos en 4 columnas ---
  
  // Columna 1: Temperatura AHT20
  Serial.print(t_aht, 1);
  Serial.print(F(" °C\t\t|\t"));

  // Columna 2: Temperatura BMP180
  Serial.print(t_bmp, 1);
  Serial.print(F(" °C\t\t|\t"));

  // Columna 3: Temperatura LM35
  Serial.print(t_lm35, 1);
  Serial.print(F(" °C\t\t|\t"));

  // Columna 4: Humedad Relativa AHT20
  Serial.print(h_aht, 1);
  Serial.println(F(" %"));

  // Esperar 2 segundos entre lecturas
  delay(2000);
}