#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

// --- Definiciones de Pines ---
const int RECV_PIN = 11;     // Pin del sensor IR
const int led = 7;     // Pin del sensor IR (corregido comentario, era LED)
const int BUZZER_PIN = 5;   // Pin del buzzer

// --- Configuración del LCD I2C ---
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C: 0x27 o 0x3F

// --- Configuración del receptor IR ---
IRrecv irrecv(RECV_PIN);
decode_results results;

// --- Configuración del Menú ---
const int NUM_OPCIONES = 3;
const char* opciones[NUM_OPCIONES] = {
  "1. Encender LED",
  "2. Ver Tiempo",
  "3. Ajustes"
};
int opcionActual = 0;

// --- Códigos IR (ajusta a tu control remoto) ---
unsigned long CODIGO_ARRIBA  = 0xFF30CF;  // Botón "ARRIBA"
unsigned long CODIGO_ABAJO   = 0xFF18E7;  // Botón "ABAJO"
unsigned long CODIGO_SELECT  = 0xFF7A85;  // Botón "OK"

// --- Función: Pitido de confirmación (sin tone) ---
void tonoConfirmacion() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
}

// --- Función: Mostrar Menú ---
void mostrarMenuSimple() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU PRINCIPAL:");

  lcd.setCursor(0, 1);
  for (int i = 0; i < NUM_OPCIONES; i++) {
    if (i == opcionActual) {
      lcd.print("[");
      lcd.print(opciones[i][0]); // solo el número
      lcd.print("]");
    } else {
      lcd.print(" ");
      lcd.print(opciones[i][0]);
      lcd.print(" ");
    }
    lcd.print(" ");
  }
}

// --- Función: Ejecutar Opción ---
void ejecutarOpcion(int opcion) {
  tonoConfirmacion();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Elegiste:");
  lcd.setCursor(0, 1);
  lcd.print(opciones[opcion]);
  delay(1500);

  switch (opcion) {
    case 0:
      lcd.clear();
      digitalWrite(led, HIGH);
      lcd.print("LED encendido!");
      delay(2000);
      digitalWrite(led, LOW);
      break;
    case 1:
      lcd.clear();
      lcd.print("Tiempo: 8.00 am ");
      break;
    case 2:
      lcd.clear();
      lcd.print("Entrando ajustes");
      break;
  }

  delay(1500);
  mostrarMenuSimple();
}

// --- SETUP ---
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Iniciando...");
  delay(1000);
  lcd.clear();

  pinMode(BUZZER_PIN, OUTPUT);

  irrecv.enableIRIn();  // Inicializa el receptor IR
  mostrarMenuSimple();
  pinMode(led,OUTPUT);

}

// --- LOOP ---
void loop() {
  if (irrecv.decode(&results)) {
    unsigned long codigoIR = results.value;

    if (codigoIR != 0xFFFFFFFF) {
      if (codigoIR == CODIGO_ARRIBA) {
        tonoConfirmacion(); // <-- CAMBIO AÑADIDO
        opcionActual = (opcionActual + 1) % NUM_OPCIONES;
        mostrarMenuSimple();
      }
      else if (codigoIR == CODIGO_ABAJO) {
        tonoConfirmacion(); // <-- CAMBIO AÑADIDO
        opcionActual = (opcionActual - 1 + NUM_OPCIONES) % NUM_OPCIONES;
        mostrarMenuSimple();
      }
      else if (codigoIR == CODIGO_SELECT) {
        ejecutarOpcion(opcionActual);
      }
    }
    irrecv.resume();
  }
}