#include <Wire.h>
#include <Adafruit_BMP085.h> // Librería directa para BMP180

Adafruit_BMP085 bmp;

unsigned long prevMillis60s = 0;
unsigned long prevMillisLectura = 0;

const unsigned long intervalo60s = 180000;    // Intervalo de 60s para el cálculo
const unsigned long intervaloLectura = 2000;  // Muestreo del sensor cada 2 segundos

float Tmin = 99.0;
float Tmax = -99.0;
float TB = 10.0; // Temperatura base para frijol según el guion
float UC_total = 0;

bool maduro = false;

void setup() {
  Serial.begin(9600);
  
  if (!bmp.begin()) {
    Serial.println("No se detectó el sensor BMP180. ¡Revisa tus conexiones!");
    while (1);
  }
  Serial.println("Sensor BMP180 detectado correctamente.");
  Serial.println("Monitoreando Unidades Calor para Frijol 'Rojo de Seda'...");
}

void loop() {
  if (maduro) return; // Detener ejecución al llegar a maduración completa

  unsigned long currentMillis = millis();

  // 1. MUESTREO DE TEMPERATURA (Cada 2 segundos sin bloquear con delay)
  if (currentMillis - prevMillisLectura >= intervaloLectura) {
    prevMillisLectura = currentMillis;
    
    float temperatura = bmp.readTemperature();

    if (!isnan(temperatura)) {
      if (temperatura < Tmin) Tmin = temperatura;
      if (temperatura > Tmax) Tmax = temperatura;
    } else {
      Serial.println("Error de lectura en el sensor BMP180.");
    }
  }

  // 2. CÁLCULO DE UNIDADES CALOR Y EVALUACIÓN FENOLÓGICA (Cada 60 segundos)
  if (currentMillis - prevMillis60s >= intervalo60s) {
    prevMillis60s = currentMillis;

    // Asegurar que hubo lecturas válidas en el minuto
    if (Tmin != 99.0 && Tmax != -99.0) {
      
      // Corregido: Variable sin acento para evitar errores de compilación
      float UC_formula = ((Tmax + Tmin) / 2.0) - TB;
      if (UC_formula < 0) UC_formula = 0; // Si la temperatura es menor a TB, no hay acumulación

      // Escalar la porción del minuto a término diario (1 día = 1440 minutos) para el acumulador
      float UC_fraccion = UC_formula / 1440.0; 
      UC_total += UC_fraccion;

      // --- IMPRESIÓN DEL REPORTE OFICIAL ---
      Serial.println("\n========= REPORTE DE UNIDADES CALOR =========");
      Serial.print("Tmin registrada: "); Serial.print(Tmin); Serial.println(" °C");
      Serial.print("Tmax registrada: "); Serial.print(Tmax); Serial.println(" °C");
      
      // Muestra el valor directo de la fórmula matemática de tu guion
      Serial.print("UC: "); Serial.println(UC_formula);
      
      // Muestra el acumulado escalado para avanzar en las etapas de la tabla
      Serial.print("UC Total Acumulado (Grados-Día): "); Serial.print(UC_total, 4); Serial.println(" UC");

      // Evaluación de la etapa fenológica basada estrictamente en la tabla "Rojo de Seda"
      Serial.print("Etapa fenológica: ");
      if (UC_total <= 12.89) {
        Serial.println("Emergencia");
      } 
      else if (UC_total <= 64.41) {
        Serial.println("Formación de guías");
      } 
      else if (UC_total <= 91.33) {
        Serial.println("Floración");
      } 
      else if (UC_total <= 99.31) {
        Serial.println("Formación de vaina");
      } 
      else if (UC_total <= 112.77) {
        Serial.println("Llenado de Vainas");
      } 
      else if (UC_total <= 153.24) {
        Serial.println("Maduración");
      } 
      else {
        Serial.println("Maduración Completa");
        Serial.println("============================================");
        Serial.println("Ciclo de cultivo terminado. Deteniendo mediciones.");
        maduro = true; 
      }
      
      if (!maduro) {
        Serial.println("============================================");
      }
    }

    // Reiniciar Tmin y Tmax para el nuevo ciclo de monitoreo
    Tmin = 99.0;
    Tmax = -99.0;
  }
}