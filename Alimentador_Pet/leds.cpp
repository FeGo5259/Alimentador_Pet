#include <Arduino.h>
#include "leds.h"

const int LED_UMIDADE = 27; // pino na esp, pode mudar 
const int LED_NIVEL = 14; // pino na esp, pode mudar

void inicializarLeds() {
  pinMode(LED_UMIDADE, OUTPUT);
  pinMode(LED_NIVEL, OUTPUT);
  digitalWrite(LED_UMIDADE, LOW);
  digitalWrite(LED_NIVEL, LOW);
}

void ledUmidadeOn() {
  digitalWrite(LED_UMIDADE, HIGH);
}

void ledUmidadeOff() {
  digitalWrite(LED_UMIDADE, LOW);
}

void ledNivelOn() {
  digitalWrite(LED_NIVEL, HIGH);
}

void ledNivelOff() {
  digitalWrite(LED_NIVEL, LOW);
}
