2. Diseñe un algoritmo que cuente la cantidad de pasos.

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float magnitude = 0;
int stepCount = 0;
const float thresholdHigh = 13.0; 
const float thresholdLow = 9.0;   
bool stepFlag = false;

void setup() {
  // Asegúrate de que el Monitor Serie esté a 115200
  Serial.begin(115200);
  while (!Serial) delay(10); // Espera a que se abra el monitor

  Serial.println("Iniciando sistema...");

  if (!mpu.begin()) {
    Serial.println("¡ERROR! No se detectó el MPU6050 en A4/A5.");
    Serial.println("Revisa: 1. Cables SDA/SCL. 2. Alimentación 5V.");
    while (1) delay(10); 
  }

  Serial.println("MPU6050 Conectado exitosamente.");
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculamos la magnitud
  magnitude = sqrt(pow(a.acceleration.x, 2) + 
                   pow(a.acceleration.y, 2) + 
                   pow(a.acceleration.z, 2));

  // Imprimimos la magnitud TODO EL TIEMPO para ver si hay señal
  Serial.print("Aceleracion Actual: ");
  Serial.print(magnitude);
  Serial.print(" | Pasos: ");
  Serial.println(stepCount);

  if (magnitude > thresholdHigh && !stepFlag) {
    stepFlag = true;
  }

  if (magnitude < thresholdLow && stepFlag) {
    stepCount++;
    stepFlag = false;
    Serial.println(">>>> ¡PASO DETECTADO! <<<<");
  }

  delay(100); 
}
