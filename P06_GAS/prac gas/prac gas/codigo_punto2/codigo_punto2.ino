const int pinMQ3 = A0;
int aireLimpio = 150; // Valor base inicial (se auto-ajustará en el setup)

void setup() {
  Serial.begin(9600);
  pinMode(pinMQ3, INPUT);
  
  Serial.println("=============================================");
  Serial.println("     INICIANDO ALCOHOLÍMETRO (mg/L)          ");
  Serial.println("=============================================");
  Serial.println("Deje el sensor en aire limpio para calibrar...");
  
  // AUTO-CALIBRACIÓN: Toma 5 lecturas seguidas para promediar el "cero" real
  long suma = 0;
  for(int i = 0; i < 5; i++) {
    suma += analogRead(pinMQ3);
    delay(1000);
  }
  aireLimpio = suma / 5;
  
  Serial.print("Calibración exitosa. Valor base actual: ");
  Serial.println(aireLimpio);
  Serial.println("---------------------------------------------");
}

void loop() {
  // 1. Leer el valor analógico crudo (0 - 1023)
  int valorCrudo = analogRead(pinMQ3);
  
  // 2. Calcular la lectura neta (restando el aire limpio ambiental)
  int lecturaNeta = valorCrudo - aireLimpio;
  if (lecturaNeta < 0) lecturaNeta = 0; // Evita valores negativos
  
  // 3. Convertir a mg/L (Miligramos de alcohol por litro de aire)
  // El factor 0.0017 aproxima la concentración directamente en el aliento
  float mgL_Aire = lecturaNeta * 0.0017;
  
  // 4. Mostrar resultados en el Monitor Serie
  Serial.print("Lectura Cruda: ");
  Serial.print(valorCrudo);
  Serial.print("  |  Alcohol en Aire: ");
  Serial.print(mgL_Aire, 3); // Muestra 3 decimales
  Serial.println(" mg/L");
  
  // 5. Diagnóstico según límites comunes en aire espirado (mg/L)
  if (mgL_Aire < 0.05) {
    Serial.println("Estado: Sobrio / Aire Limpio");
  } 
  else if (mgL_Aire >= 0.05 && mgL_Aire < 0.25) {
    Serial.println("Estado: [PRECAUCIÓN] Presencia moderada de alcohol");
  } 
  else {
    Serial.println("Estado: [¡ALERTA!] Límite legal superado (Posible positivo)");
  }
  
  Serial.println("---------------------------------------------");
  delay(1500); // Actualiza la lectura cada 1.5 segundos
}