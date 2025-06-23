#include <Arduino.h>
#include "sensores.h"

const int TRIG_PIN = 32; //pinos na esp, podem mudar
const int ECHO_PIN = 33;
const int PIN_UMIDADE = 34;

// Distância de referência quando a ração está presente (ex: < 10cm)
// Ajuste esse valor baseado na sua montagem real
const int DISTANCIA_MAX_RACAO_CM = 10;

void inicializarSensores() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIN_UMIDADE, INPUT);
}

bool umidadeAlto() {
  int leitura = analogRead(PIN_UMIDADE);
  return leitura > 2000; // ajuste conforme seu sensor
}

// Realiza a medição ultrassônica (em cm)
long medirDistanciaCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 0.034 / 2;
  return distancia;
}

// Média de várias medições para suavizar ruído
long medirDistanciaMediaCM() {
  long soma = 0;
  const int numMedidas = 5;

  for (int i = 0; i < numMedidas; i++) {
    soma += medirDistanciaCM();
    delay(50);
  }

  return soma / numMedidas;
}

// Retorna true se o nível estiver abaixo do desejado (ou seja, se a distância for maior)
bool nivelBaixo() {
  long distancia = medirDistanciaMediaCM();
  return distancia > DISTANCIA_MAX_RACAO_CM;
}
