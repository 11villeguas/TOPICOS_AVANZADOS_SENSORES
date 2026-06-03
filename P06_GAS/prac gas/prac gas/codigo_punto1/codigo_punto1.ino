const int pinMQ2 = A0;  // Pin analógico donde está conectado el sensor

// Valores de calibración (ajustables según tu entorno)
const int AIRE_LIMPIO = 80;   // Valor analógico promedio en un cuarto ventilado (aprox. 0%)
const int SATURACION = 900;   // Valor analógico estimado ante una fuga fuerte (aprox. 100%)

void setup() {
  Serial.begin(9600);
  pinMode(pinMQ2, INPUT);
  
  Serial.println("=========================================");
  Serial.println("Estabilizando sensor MQ-2 (Espere...)");
  Serial.println("=========================================");
  delay(10000); // 10 segundos de espera inicial
}

void loop() {
  // 1. Leer el valor analógico crudo (0 a 1023)
  int valorCrudo = analogRead(pinMQ2);
  
  // 2. Convertir el valor a porcentaje (0% - 100%) usando la función map
  // constrain evita que el porcentaje sea menor a 0 o mayor a 100
  int valorLimitado = constrain(valorCrudo, AIRE_LIMPIO, SATURACION);
  int porcentajeGas = map(valorLimitado, AIRE_LIMPIO, SATURACION, 0, 100);
  
  // 3. Mostrar los resultados en el Monitor Serie
  Serial.print("Lectura Cruda: ");
  Serial.print(valorCrudo);
  Serial.print("  |  Concentración General: ");
  Serial.print(porcentajeGas);
  Serial.println("%");

  // 4. Sistema de alertas visuales en el monitor
  if (porcentajeGas < 20) {
    Serial.println("Estado: Aire Limpio / Normal");
  } 
  else if (porcentajeGas >= 20 && porcentajeGas < 50) {
    Serial.println("Estado: [PRECAUCIÓN] Presencia leve de gas o humo");
  } 
  else {
    Serial.println("Estado: [¡ALERTA!] Alta concentración de gas");
  }
  

  delay(1500); // Actualiza la lectura cada 1.5 segundos
}