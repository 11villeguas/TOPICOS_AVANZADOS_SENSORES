#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

// --- AJUSTE ESPECÍFICO PARA CALERA, ZACATECAS ---
// La presión estándar al nivel del mar es 101325 Pa.
// Si notas un desfase por el clima actual, puedes ajustar ligeramente este valor.
float presion_nivel_mar = 101325; 

void setup() {
  Serial.begin(9600);
  
  // Inicializar el sensor BMP180
  if (!bmp.begin()) {
    Serial.println("¡Error! No se encuentra el sensor BMP180. Verifica SDA (A4) y SCL (A5).");
    while (1) {} 
  }
  
  Serial.println("--- Sensor BMP180 Inicializado ---");
  Serial.println("Configurado para Calera, Zacatecas.");
  Serial.println("----------------------------------------");
}

void loop() {
  float temperatura = bmp.readTemperature();
  float presion_actual = bmp.readPressure();
  
  // Calcula la altitud calculando el diferencial con la presión del nivel del mar
  float altitud = bmp.readAltitude(presion_nivel_mar);

  // Mostrar datos en el Monitor Serie
  Serial.print("Temperatura Local: ");
  Serial.print(temperatura);
  Serial.println(" *C");

  Serial.print("Presion Atmosferica: ");
  Serial.print(presion_actual / 100.0); // Mostrado en hPa
  Serial.println(" hPa");

  Serial.print("Altitud Calculada: ");
  Serial.print(altitud);
  Serial.println(" metros sobre el nivel del mar");

  Serial.println("----------------------------------------");
  
  delay(2500); // Lectura cada 2.5 segundos
}