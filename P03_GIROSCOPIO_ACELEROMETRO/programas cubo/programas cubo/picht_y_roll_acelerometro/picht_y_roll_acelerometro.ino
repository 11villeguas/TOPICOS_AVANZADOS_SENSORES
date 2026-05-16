//tilizando el sensor MPU6050 obtener pitch y roll usando solo giroscopio

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float gyro_roll = 0, gyro_pitch = 0;
float offset_x = 0, offset_y = 0;
unsigned long tiempo_prev;

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) {
    while (1) delay(10);
  }

  // --- CALIBRACIÓN AUTOMÁTICA ---
  // Lee el error inicial mientras el sensor está quieto
  Serial.println("Calibrando... No muevas el sensor");
  for(int i = 0; i < 200; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    offset_x += g.gyro.x;
    offset_y += g.gyro.y;
    delay(5);
  }
  offset_x /= 200;
  offset_y /= 200;
  Serial.println("Calibración lista.");

  tiempo_prev = millis();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  unsigned long tiempo_actual = millis();
  float dt = (tiempo_actual - tiempo_prev) / 1000.0;
  tiempo_prev = tiempo_actual;

  // Restamos el offset para que cuando esté quieto marque 0
  float gx = (g.gyro.x - offset_x) * 180 / PI;
  float gy = (g.gyro.y - offset_y) * 180 / PI;

  // Integración: ángulo = ángulo + (velocidad * tiempo)
  gyro_roll  += gx * dt;
  gyro_pitch += gy * dt;

  Serial.print("Giro_Roll: "); Serial.print(gyro_roll, 2);
  Serial.print(" | Giro_Pitch: "); Serial.println(gyro_pitch, 2);

  delay(10); 
}