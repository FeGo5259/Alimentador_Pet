#include <Arduino.h>
#include "motor.h"

// --- Configuração dos Pinos para o Motor de Passo de 4 Fios ---
#define STEP_PIN_1 14
#define STEP_PIN_2 27
#define STEP_PIN_3 26
#define STEP_PIN_4 25

// --- Constantes do Sistema ---
const int GRAMAS_POR_ROTACAO = 5;      // Quantidade de gramas que uma rotação completa dispensa.
const int PASSOS_POR_ROTACAO = 32;   // Ajuste para o motor 
const int TEMPO_LIBERACAO_CONSTANTE_MS = 10000; // Tempo total que o motor ficará ligado (10 segundos).

// --- Variáveis de Controle ---
volatile bool motorLigado = false;
static int porcaoAtualGramas = 20; // Porção padrão de 20g.
static int delayPorPasso_ms = 2;   // Delay entre cada fase do passo. Será calculado dinamicamente.

void inicializarMotor() {
  pinMode(STEP_PIN_1, OUTPUT);
  pinMode(STEP_PIN_2, OUTPUT);
  pinMode(STEP_PIN_3, OUTPUT);
  pinMode(STEP_PIN_4, OUTPUT);
}

void setPorcao(int gramas) {
  if (gramas > 0) {
    porcaoAtualGramas = gramas;
  }
}

void ligarMotor() {
  // --- Lógica de Cálculo da Velocidade ---
  // Calcula o número total de rotações necessárias.
  float totalRotacoes = (float)porcaoAtualGramas / GRAMAS_POR_ROTACAO;

  // Calcula o número total de passos do motor.
  int totalPassos = totalRotacoes * PASSOS_POR_ROTACAO;

  // Como a sequência de passos tem 4 fases, calculamos o número total de "fases".
  int totalFases = totalPassos * 4;

  // Calcula o delay necessário entre cada fase para que o tempo total seja constante.
  // Garante que o delay não seja zero para evitar divisão por zero e funcionamento incorreto.
  if (totalFases > 0) {
    delayPorPasso_ms = TEMPO_LIBERACAO_CONSTANTE_MS / totalFases;
  } else {
    delayPorPasso_ms = 2; // Um valor padrão caso a conta dê zero.
  }

  motorLigado = true;
}

void desligarMotor() {
  motorLigado = false;
  // Desliga todas as bobinas para economizar energia e evitar aquecimento.
  digitalWrite(STEP_PIN_1, LOW);
  digitalWrite(STEP_PIN_2, LOW);
  digitalWrite(STEP_PIN_3, LOW);
  digitalWrite(STEP_PIN_4, LOW);
}

// Tarefa dedicada que executa os passos do motor
void taskMotor(void *pvParameters) {
  for (;;) {
    if (motorLigado) {
      // Executa a sequência de 4 passos (full-step, uma fase por vez)
      digitalWrite(STEP_PIN_1, HIGH);
      digitalWrite(STEP_PIN_2, LOW);
      digitalWrite(STEP_PIN_3, LOW);
      digitalWrite(STEP_PIN_4, LOW);
      vTaskDelay(pdMS_TO_TICKS(delayPorPasso_ms));

      digitalWrite(STEP_PIN_1, LOW);
      digitalWrite(STEP_PIN_2, HIGH);
      vTaskDelay(pdMS_TO_TICKS(delayPorPasso_ms));

      digitalWrite(STEP_PIN_2, LOW);
      digitalWrite(STEP_PIN_3, HIGH);
      vTaskDelay(pdMS_TO_TICKS(delayPorPasso_ms));

      digitalWrite(STEP_PIN_3, LOW);
      digitalWrite(STEP_PIN_4, HIGH);
      vTaskDelay(pdMS_TO_TICKS(delayPorPasso_ms));
    } else {
      // Se o motor não estiver ligado, apenas aguarda para não consumir CPU.
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}
