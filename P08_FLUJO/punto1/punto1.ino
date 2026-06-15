// Código para caracterización de sensor de flujo (Corregido)
const int sensorPin = 2; // Pin de interrupción
volatile long pulseCount = 0;
unsigned long interval = 1000; 
unsigned long previousMillis = 0;

// Quitamos el IRAM_ATTR para compatibilidad total con Arduino AVR
void countPulse() {
  pulseCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
  // Vinculamos la interrupción
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, FALLING);
  
  Serial.println("Iniciando medicion de pulsos...");
  Serial.println("Tiempo(ms), Pulsos_por_segundo");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Deshabilitar interrupción para leer el dato de forma segura
    detachInterrupt(digitalPinToInterrupt(sensorPin));
    
    // Imprimir resultados
    Serial.print(currentMillis);
    Serial.print(", ");
    Serial.println(pulseCount);
    
    // Resetear contadores
    pulseCount = 0;
    previousMillis = currentMillis;
    
    // Re-habilitar interrupción
    attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, FALLING);
  }
}