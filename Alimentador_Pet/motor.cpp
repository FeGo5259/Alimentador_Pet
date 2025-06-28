#include <Arduino.h>
#include "motor.h"
#include "fsm.h" // Necessário para acessar a fila e os eventos

// --- Configuração dos Pinos para o Motor de Passo de 4 Fios ---
#define STEP_PIN_1 16
#define STEP_PIN_2 17
#define STEP_PIN_3 5
#define STEP_PIN_4 18
#define BUTTON_PIN 2

// --- Variáveis de Controle ---
static int porcaoAtualGramas = 50; // Porção padrão de 20g
#define delayPorPasso_ms  2   // Delay entre cada fase do passo
static long tempoLiberacaoMs = 4000; // Tempo padrão (4s para 20g)

// Disponibiliza a fila de eventos para esta tarefa
extern QueueHandle_t xQueue;

void inicializarMotor() {
  pinMode(STEP_PIN_1, OUTPUT);
  pinMode(STEP_PIN_2, OUTPUT);
  pinMode(STEP_PIN_3, OUTPUT);
  pinMode(STEP_PIN_4, OUTPUT);
}

// Calcula o tempo necessário com base no peso desejado
void setPorcao(int porcao) {
  if (porcao > 0) {
    porcaoAtualGramas = porcao;
    //200ms por grama
    tempoLiberacaoMs = porcaoAtualGramas * 200; 
  }
}

void desligarMotor() {
  // Desliga todas as bobinas para economizar energia e evitar aquecimento.
  digitalWrite(STEP_PIN_1, LOW);
  digitalWrite(STEP_PIN_2, LOW);
  digitalWrite(STEP_PIN_3, LOW);
  digitalWrite(STEP_PIN_4, LOW);
}

// Função chamada pela FSM para iniciar o processo. Retorna imediatamente.

void acionaMotor() {
  
  TickType_t tempoInicio = xTaskGetTickCount(); // Marca o tempo inicial

  // Loop principal: gira o motor enquanto o tempo não tiver expirado
  while ((xTaskGetTickCount() - tempoInicio) < pdMS_TO_TICKS(tempoLiberacaoMs)) {
    // Executa a sequência de 4 passos
    digitalWrite(STEP_PIN_1, LOW);
    digitalWrite(STEP_PIN_2, LOW);
    digitalWrite(STEP_PIN_3, LOW);
    digitalWrite(STEP_PIN_4, HIGH);
    vTaskDelay(pdMS_TO_TICKS(delayPorPasso_ms));

    digitalWrite(STEP_PIN_4, LOW);
    digitalWrite(STEP_PIN_3, HIGH);
    vTaskDelay(pdMS_TO_TICKS(delayPorPasso_ms));

    digitalWrite(STEP_PIN_3, LOW);
    digitalWrite(STEP_PIN_2, HIGH);
    vTaskDelay(pdMS_TO_TICKS(delayPorPasso_ms));

    digitalWrite(STEP_PIN_2, LOW);
    digitalWrite(STEP_PIN_1, HIGH);
    vTaskDelay(pdMS_TO_TICKS(delayPorPasso_ms));
  }

  // Envia o evento de conclusão para a máquina de estados UMA ÚNICA VEZ
  int evento = EVENTO_PORCAO_LIBERADA;
  xQueueSend(xQueue, &evento, 0);

}

void setupBotao(){
  pinMode(BUTTON_PIN,INPUT);
}

void taskBotao(void *pvParameters){
bool estadoAnterior = false;
  for(;;){
    bool pressionado = digitalRead(BUTTON_PIN);
    if (pressionado && !estadoAnterior){
      // Detectou transição de LOW → HIGH (borda de subida)
      int evento = EVENTO_BOTAO_MANUAL;
      xQueueSend(xQueue, &evento, 0);
    }
    estadoAnterior = pressionado;
    vTaskDelay(pdMS_TO_TICKS(30));
  }
}
