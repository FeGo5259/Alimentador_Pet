#include <Arduino.h>
#include "motor.h"

const int PIN_MOTOR = 26;

void ligarMotor() {
  digitalWrite(PIN_MOTOR, HIGH);
}

void desligarMotor() {
  digitalWrite(PIN_MOTOR, LOW);
}

void inicializarMotor() {
  pinMode(PIN_MOTOR, OUTPUT);
  desligarMotor();
}
