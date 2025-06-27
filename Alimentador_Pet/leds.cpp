#include <Arduino.h>
#include "leds.h"

#define LED_UMIDADE 22
#define LED_NIVEL 23


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
