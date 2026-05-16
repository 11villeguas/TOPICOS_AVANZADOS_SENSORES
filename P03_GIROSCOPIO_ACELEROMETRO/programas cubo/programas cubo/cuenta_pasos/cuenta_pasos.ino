//3. Diseñe un algoritmo simulando la activación de una bolsa de aire, considere lo siguiente:

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Pin del indicador de activación (LED o Buzzer)
const int airbagPin = 13;

// Variables para el filtro de cambios repentinos
float ax_prev = 0, ay_prev = 0, az_prev = 0;
bool airbagDeployed = false;

// --- CONFIGURACIÓN DEL UMBRAL ---
// Umbral de cambio de aceleración (Jerk) en m/s^2.
// Un valor de 80 m/s^2 es aprox 8 G's de cambio repentino.
// Ajústalo: más alto = requiere golpe más fuerte.
const float crashThreshold = 80.0; 

void setup() {
  Serial.begin(115200);
  pinMode(airbagPin, OUTPUT);
  digitalWrite(airbagPin, LOW); // Asegurar que esté apagado al inicio

  if (!mpu.begin()) {
    Serial.println("Error: MPU6050 no encontrado.");
    while (1) delay(10);
  }

  // Configurar para máxima capacidad de medición de impacto (16G)
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  
  // Desactivar filtros internos para tener la respuesta más rápida posible (muestreo puro)
  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ); 

  Serial.println("SISTEMA DE AIRBAG ARMADO.");
  Serial.println("Simula un choque dando un golpe seco al sensor.");
  
  // Leer valores iniciales para el filtro
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  ax_prev = a.acceleration.x;
  ay_prev = a.acceleration.y;
  az_prev = a.acceleration.z;
}

void loop() {
  // Si ya se activó, no hacer nada más (requiere reset físico)
  if (airbagDeployed) {
    return; 
  }

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // 1. Calcular el CAMBIO de aceleración en cada eje (Filtro Pasa-Altas básico)
  // Esto elimina la gravedad y detecta solo el impacto.
  float delta_ax = a.acceleration.x - ax_prev;
  float delta_ay = a.acceleration.y - ay_prev;
  float delta_az = a.acceleration.z - az_prev;

  // Actualizar valores anteriores para el próximo ciclo
  ax_prev = a.acceleration.x;
  ay_prev = a.acceleration.y;
  az_prev = a.acceleration.z;

  // 2. Calcular la Magnitud Resultante del cambio (Pitágoras)
  float impulseMagnitude = sqrt(pow(delta_ax, 2) + pow(delta_ay, 2) + pow(delta_az, 2));

  // Opcional: Ver la fuerza del golpe en el monitor serie
  if (impulseMagnitude > 10) { // Solo imprimir si es un golpe relevante
    Serial.print("Fuerza de impacto detectada: ");
    Serial.print(impulseMagnitude);
    Serial.println(" m/s^2");
  }

  // 3. COMPARACIÓN CON EL UMBRAL DE CRASH
  if (impulseMagnitude > crashThreshold) {
    deployAirbag();
  }

  // Muestreo muy rápido para no perder el pico del choque
  delay(1); 
}

void deployAirbag() {
  airbagDeployed = true;
  
  // ACTIVACIÓN INSTANTÁNEA
  digitalWrite(airbagPin, HIGH); 
  
  // Enviar alerta al sistema central
  Serial.println("\n######################################");
  Serial.println("!!! IMPACTO CRÍTICO DETECTADO !!!");
  Serial.println(">>> ACTIVANDO BOLSA DE AIRE <<<");
  Serial.println("Tiempo de decisión: < 2ms");
  Serial.println("######################################");
  
  // En un sistema real, aquí se activaría el pirotécnico.
  // Mantenemos el LED encendido para indicar que el sistema quedó inutilizable.
}
