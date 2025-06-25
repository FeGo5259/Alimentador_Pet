#ifndef SENSORES_H
#define SENSORES_H

void inicializarSensores();
bool umidadeAlto();     // retorna true se umidade > 50%
bool nivelBaixo();      // retorna true se nível estiver abaixo do ideal
void taskVerificaSensores(void *pvParameters);

#endif
#include <Arduino.h>
#include "sensores.h"
#include "fsm.h"

const int PIN_SENSOR_OBSTACULO = 32;
const int PIN_UMIDADE = 34;

void inicializarSensores() {
  pinMode(PIN_SENSOR_OBSTACULO, INPUT);
  pinMode(PIN_UMIDADE, INPUT);
}

bool umidadeAlto() {
  int leitura = analogRead(PIN_UMIDADE);
  return leitura > 2000; // ajuste conforme seu sensor
}

// Retorna true se o nível estiver abaixo do desejado (ou seja, se a distância for maior)
bool nivelBaixo() {
  int leitura = digitalRead(PIN_SENSOR_OBSTACULO);
  // Se o sensor *não detectar* obstáculo (retorna HIGH), consideramos nível baixo
  return leitura == HIGH;
}

void taskVerificaSensores(void *pvParameters) {
  static bool estadoUmidadeAnterior = false;
  static bool estadoNivelAnterior = false;

  for (;;) {
    bool umidadeAltaAtual = umidadeAlto();
    bool nivelBaixoAtual = nivelBaixo();

    if (umidadeAltaAtual != estadoUmidadeAnterior) {
      int evento = umidadeAltaAtual ? EVENTO_UMIDADE_ALTA : EVENTO_UMIDADE_NORMAL;
      xQueueSend(xQueue, &evento, 0);
      estadoUmidadeAnterior = umidadeAltaAtual;
    }

    if (nivelBaixoAtual != estadoNivelAnterior) {
      int evento = nivelBaixoAtual ? EVENTO_NIVEL_BAIXO : EVENTO_NIVEL_NORMAL;
      xQueueSend(xQueue, &evento, 0);
      estadoNivelAnterior = nivelBaixoAtual;
    }
  }
}