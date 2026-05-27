#include <Wire.h>
#include <Adafruit_AHTX0.h>

// Crear el objeto para el sensor AHT20
Adafruit_AHTX0 aht;

// Variables para el contador de segundos con millis()
unsigned long tiempoAnterior = 0;
unsigned long segundosTranscurridos = 0;
bool cronometroActivo = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10); // Esperar a que se abra el monitor serie

  Serial.println(F("--- Inicializando Sensor AHT20 ---"));

  if (!aht.begin()) {
    Serial.println(F("¡Error! No se pudo encontrar el sensor AHT20. Revisa el cableado."));
    while (1) delay(10);
  }
  Serial.println(F("AHT20 detectado y listo."));
  Serial.println(F("--------------------------------------------------"));
  
  // Encabezado de las columnas
  Serial.println(F("Humedad Real\t\t| Tiempo Transcurrido"));
  Serial.println(F("--------------------------------------------------"));
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  float humedadActual = humidity.relative_humidity;

  // --- LÓGICA DEL CRONÓMETRO ---
  if (!cronometroActivo && humedadActual >= 40.0) {
    cronometroActivo = true;
    tiempoAnterior = millis(); 
  }

  if (cronometroActivo) {
    if (millis() - tiempoAnterior >= 1000) {
      segundosTranscurridos++;
      tiempoAnterior += 1000; 
    }
  }

  // --- IMPRESIÓN EN FORMATO DE COLUMNAS ---
  
  // Columna 1: Humedad
  Serial.print(humedadActual, 1);
  Serial.print(F(" %"));
  
  // Separador de columnas (Tabulador y barra)
  Serial.print(F("\t\t|\t"));

  // Columna 2: Tiempo (Solo muestra los segundos si ya se activó)
  if (cronometroActivo) {
    Serial.print(segundosTranscurridos);
    Serial.println(F(" seg"));
  } else {
    Serial.println(F("Esperando umbral (40%)..."));
  }

  // Esperar 1 segundo para que el contador avance de forma fluida
  delay(1000);
}