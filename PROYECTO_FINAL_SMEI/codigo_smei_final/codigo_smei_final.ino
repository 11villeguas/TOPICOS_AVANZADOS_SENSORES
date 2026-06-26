#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int PIN_TRIG = 2;
const int PIN_ECHO = 3;
const int PIN_BOTON = 4;
const int PIN_BUZZER = 5;

const float ALTURA_MINIMA = 50.0;
const float UMBRAL_ACELERACION = 2.5;
const float UMBRAL_INCLINACION = 45.0;

const unsigned long TIEMPO_HOMBRE_MUERTO = 30000;
const unsigned long TIEMPO_ALERTA_MAXIMA = 30000;

unsigned long ultimaVezPresionado = 0;
unsigned long inicioPitidoIntermitente = 0;
unsigned long ultimoCambioBuzzer = 0;
unsigned long ultimoReporteSerie = 0;

enum EstadoHombreMuerto { OK, INTERMITENTE, CRITICO };
EstadoHombreMuerto estadoActual = OK;

bool sistemaActivo = false;
bool buzzerIntermitenteEstado = false;

int contadorClicks = 0;
unsigned long ultimoClickTime = 0;
bool ultimoEstadoBoton = HIGH;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Wire.begin();
  mpu.initialize();
  
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() {
  float altura = obtenerDistanciaFiltrada();
  bool botonPresionado = revisarBoton();

  if (millis() - ultimoReporteSerie >= 1500) {
    ultimoReporteSerie = millis();
    Serial.print("Distancia: "); 
    if (altura > 900) Serial.print("ERROR/LECTURA BAJA"); 
    else { Serial.print(altura); Serial.print(" cm"); }
    
    Serial.print(" | Boton: "); Serial.print(digitalRead(PIN_BOTON) == LOW ? "PRESIONADO" : "SUELTO");
    Serial.print(" | Sistema: "); Serial.println(sistemaActivo ? "ACTIVO" : "APAGADO (<50cm)");
  }

  if (altura > ALTURA_MINIMA && altura < 400.0) {
    if (!sistemaActivo) {
      sistemaActivo = true;
      ultimaVezPresionado = millis();
      estadoActual = OK;
    }
  } else if (altura <= ALTURA_MINIMA) {
    if (sistemaActivo) {
      desactivarSistema();
    }
  }

  if (botonPresionado) {
    gestionarClickBoton();
  }

  if (sistemaActivo) {
    revisarMPU();

    unsigned long tiempoTranscurrido = millis() - ultimaVezPresionado;

    switch (estadoActual) {
      case OK:
        noTone(PIN_BUZZER);
        if (tiempoTranscurrido >= TIEMPO_HOMBRE_MUERTO) {
          estadoActual = INTERMITENTE;
          inicioPitidoIntermitente = millis();
        }
        break;

      case INTERMITENTE:
        sonarBuzzerIntermitente();
        if (millis() - inicioPitidoIntermitente >= TIEMPO_ALERTA_MAXIMA) {
          estadoActual = CRITICO;
          enviarNotificacionCelular("¡EMERGENCIA! El operador no responde al botón de seguridad.");
        }
        break;

      case CRITICO:
        tone(PIN_BUZZER, 1000);
        break;
    }
  } else {
    noTone(PIN_BUZZER);
    contadorClicks = 0; 
  }
}

float obtenerDistanciaFiltrada() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  long duracion = pulseIn(PIN_ECHO, HIGH, 25000); 
  float distancia = duracion * 0.034 / 2;
  
  if (distancia <= 0 || distancia > 400) return 999; 
  return distancia;
}

bool revisarBoton() {
  bool lectura = digitalRead(PIN_BOTON);
  if (lectura == LOW && ultimoEstadoBoton == HIGH) { 
    delay(50);
    ultimoEstadoBoton = lectura;
    return true;
  }
  ultimoEstadoBoton = lectura;
  return false;
}

void gestionarClickBoton() {
  unsigned long tiempoAhora = millis();
  
  if (tiempoAhora - ultimoClickTime > 1500) {
    contadorClicks = 0;
  }
  
  contadorClicks++;
  ultimoClickTime = tiempoAhora;
  
  ultimaVezPresionado = tiempoAhora;
  if (estadoActual != CRITICO) {
    estadoActual = OK; 
  }

  if (contadorClicks >= 3) {
    desactivarSistema();
  }
}

void revisarMPU() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  double gTotal = sqrt(pow(ax, 2) + pow(ay, 2) + pow(az, 2)) / 16384.0; 

  float inclinacionX = atan2(ay, az) * 180 / M_PI;
  float inclinacionY = atan2(ax, az) * 180 / M_PI;

  if (gTotal > UMBRAL_ACELERACION) {
    dispararAlertaInmediata("¡CAÍDA BRUSCA DETECTADA!");
  }

  if (abs(inclinacionX) > UMBRAL_INCLINACION || abs(inclinacionY) > UMBRAL_INCLINACION) {
    dispararAlertaInmediata("¡POSTURA PELIGROSA INCLINADA!");
  }
}

void sonarBuzzerIntermitente() {
  if (millis() - ultimoCambioBuzzer >= 250) { 
    ultimoCambioBuzzer = millis();
    buzzerIntermitenteEstado = !buzzerIntermitenteEstado;
    if (buzzerIntermitenteEstado) {
      tone(PIN_BUZZER, 1200);
    } else {
      noTone(PIN_BUZZER);
    }
  }
}

void dispararAlertaInmediata(String motivo) {
  enviarNotificacionCelular(motivo);
  estadoActual = CRITICO; 
}

void desactivarSistema() {
  sistemaActivo = false;
  estadoActual = OK;
  noTone(PIN_BUZZER);
  contadorClicks = 0;
}

void enviarNotificacionCelular(String mensaje) {
  Serial.print("ALERTA_CELULAR:");
  Serial.println(mensaje);
}