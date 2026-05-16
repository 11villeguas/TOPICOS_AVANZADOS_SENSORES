#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
    // Iniciamos comunicación serial
    Serial.begin(9600);
    Wire.begin();
    
    // Inicializar el sensor
    mpu.initialize();

    // Rango ±8g (Cada 1g equivale a 4096 unidades)
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);

    // Verificar si el sensor responde
    if (!mpu.testConnection()) {
        Serial.println("Error: MPU6050 no detectado");
        while (1);
    }
    
    Serial.println("SISTEMA LISTO. Mueve el sensor...");
}

void loop() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Convertir a Gravedades (g)
    float accel_x = ax / 4096.0;
    float accel_y = ay / 4096.0;
    float accel_z = az / 4096.0;

    // Valores absolutos para saber qué eje tiene más peso
    float absX = abs(accel_x);
    float absY = abs(accel_y);
    float absZ = abs(accel_z);

    String face = ""; 

    // Lógica para detectar qué cara está hacia arriba
    if (absX > absY && absX > absZ) {
        if (accel_x > 0.5) face = "FRENTE";
        else if (accel_x < -0.5) face = "ATRAS";
    } 
    else if (absY > absX && absY > absZ) {
        if (accel_y > 0.5) face = "IZQUIERDA";
        else if (accel_y < -0.5) face = "DERECHA";
    } 
    else {
        if (accel_z > 0.5) face = "ARRIBA";
        else if (accel_z < -0.5) face = "ABAJO";
    }

    // Imprimir solo si se detectó una cara clara
    if (face != "") {
        Serial.print("Posicion: ");
        Serial.println(face);
    }

    delay(300); // Pausa para que puedas leer los mensajes
}