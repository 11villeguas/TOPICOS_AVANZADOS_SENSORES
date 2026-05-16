#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// --- NUEVAS VARIABLES DE CONTROL ---
int tarea = 0;           // ID de la tarea actual (1-6)
int contador_tarea = 0;  // Valor del contador
String ultima_face = ""; // Para detectar el cambio de posición

void setup() {
    Serial.begin(9600);
    Wire.begin();
    
    mpu.initialize();
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);

    if (!mpu.testConnection()) {
        Serial.println("Error: MPU6050 no detectado");
        while (1);
    }
    
    Serial.println("SISTEMA LISTO. Mueve el sensor...");
}

void loop() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    float accel_x = ax / 4096.0;
    float accel_y = ay / 4096.0;
    float accel_z = az / 4096.0;

    float absX = abs(accel_x);
    float absY = abs(accel_y);
    float absZ = abs(accel_z);

    String face = ""; 
    int tarea_detectada = 0;

    // --- LÓGICA DE DETECCIÓN Y ASIGNACIÓN DE TAREAS ---
    if (absX > absY && absX > absZ) {
        if (accel_x > 0.5) {
            face = "FRENTE";
            tarea_detectada = 6;
        } else if (accel_x < -0.5) {
            face = "ATRAS";
            tarea_detectada = 1;
        }
    } 
    else if (absY > absX && absY > absZ) {
        if (accel_y > 0.5) {
            face = "IZQUIERDA";
            tarea_detectada = 3;
        } else if (accel_y < -0.5) {
            face = "DERECHA";
            tarea_detectada = 4;
        }
    } 
    else {
        if (accel_z > 0.5) {
            face = "ARRIBA";
            tarea_detectada = 2;
        } else if (accel_z < -0.5) {
            face = "ABAJO";
            tarea_detectada = 5;
        }
    }

    // --- MANEJO DE CONTADORES Y REINICIO ---
    if (face != "" && face != ultima_face) {
        // Si la posición cambió, reiniciamos
        tarea = tarea_detectada;
        contador_tarea = 0; 
        ultima_face = face;
        
        Serial.println("--- CAMBIO DE POSICIÓN ---");
    } else if (face != "" && face == ultima_face) {
        // Si se mantiene la posición, incrementa
        contador_tarea++;
    }

    // --- IMPRESIÓN DE DATOS ---
    if (face != "") {
        Serial.print("Posicion: ");
        Serial.print(face);
        Serial.print(" | Tarea: ");
        Serial.print(tarea);
        Serial.print(" | Contador: ");
        Serial.println(contador_tarea);
    }

    delay(300); 
}