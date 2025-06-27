#include <Arduino.h>
#include "sensores.h"
#include "fsm.h"
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define PIN_SENSOR_OBSTACULO 34
#define DHTPIN 26
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void inicializarSensores() {
  dht.begin();
  pinMode(PIN_SENSOR_OBSTACULO, INPUT);
  pinMode(DHTPIN, INPUT);
}

bool umidadeAlto() {
  float h = dht.readHumidity();
  //Serial.printf("Umidade Lida: %.1f%%\n", h);
  
  if (isnan(h)) {
    Serial.println("Erro na leitura da umidade!");
    return false;
  }
  
  if (h > 30.0) {
    return true;
  }
  return false;
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