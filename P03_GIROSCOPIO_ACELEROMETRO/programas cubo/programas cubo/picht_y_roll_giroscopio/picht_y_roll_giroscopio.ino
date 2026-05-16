//Aplicar un filtro pasa-altas giroscopio y un filtro pasa-bajas al acelerómetro. Vuelve a calcular pitch y roll



#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float angle_pitch = 0, angle_roll = 0;
unsigned long tiempo_prev;

// Factor de filtrado (0.98 es el estándar)
float alpha = 0.98;

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) {
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  tiempo_prev = millis();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // 1. Calcular diferencial de tiempo
  unsigned long tiempo_actual = millis();
  float dt = (tiempo_actual - tiempo_prev) / 1000.0;
  tiempo_prev = tiempo_actual;

  // 2. Calcular ángulos base del ACELERÓMETRO (Pasa-Bajas natural)
  float accel_roll = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  float accel_pitch = atan2(-a.acceleration.x, sqrt(pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2))) * 180 / PI;

  // 3. Aplicar FILTRO COMPLEMENTARIO
  // El Giroscopio (Pasa-Altas) domina el corto plazo (0.98)
  // El Acelerómetro (Pasa-Bajas) domina el largo plazo (0.02) corrigiendo la deriva
  
  angle_roll = alpha * (angle_roll + (g.gyro.x * 180 / PI) * dt) + (1.0 - alpha) * accel_roll;
  angle_pitch = alpha * (angle_pitch + (g.gyro.y * 180 / PI) * dt) + (1.0 - alpha) * accel_pitch;

  // 4. Visualización
  Serial.print("Roll_Filtrado: "); Serial.print(angle_roll, 2);
  Serial.print(" | Pitch_Filtrado: "); Serial.println(angle_pitch, 2);

  delay(1000); 
}