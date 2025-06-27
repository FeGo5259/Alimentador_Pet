#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <BluetoothSerial.h>
#include <time.h>

#include "fsm.h"
#include "sensores.h"
#include "motor.h"
#include "leds.h"
#include "bluetooth.h"
#include "horarios.h"

#define MAX_HORARIOS 10


// Variáveis globais
QueueHandle_t xQueue;
void setup() {
  Serial.begin(115200);
  
  // Inicializações de hardware
  if(!SerialBT.begin("AlimentadorPet")) {
    Serial.println("Falha ao inicializar Bluetooth!");
    while(1);
  }
  setupBotao();
  setupHora();
  inicializarMotor();
  inicializarSensores();
  inicializarLeds();
  inicializarMaquinaEstados();

  // Criação da fila e tarefas
  xQueue = xQueueCreate(10, sizeof(int));
  if (xQueue != NULL) {
    // Cria as tarefas com stacks e prioridades adequadas
    xTaskCreatePinnedToCore(
      taskMaquinaEstados,    // Função da tarefa
      "FSM",                // Nome da tarefa
      4096,                 // Tamanho do stack (aumentado para segurança)
      NULL,                 // Parâmetros
      2,                    // Prioridade
      NULL,                 // Task handle
      1                     // Core (0 ou 1)
    );
    
    xTaskCreatePinnedToCore(
      taskVerificaSensores,
      "Sensores",
      2048,
      NULL,
      1,                    // Prioridade menor que FSM
      NULL,
      1
    );
    
    xTaskCreatePinnedToCore(
      bluetoothTask,
      "Bluetooth",
      4096,                // Bluetooth pode precisar de mais stack
      NULL,
      2,                    // Alta prioridade como FSM
      NULL,
      0                     // Executa no outro core
    );
    
    xTaskCreatePinnedToCore(
      taskVerificaHorario,
      "Horario",
      2048,
      NULL,
      1,
      NULL,
      1
    );

    xTaskCreatePinnedToCore(
      taskBotao,
      "Botao",
      2048,
      NULL,
      1,                    // Prioridade menor que FSM
      NULL,
      1
    );
    
    Serial.println("Tarefas criadas com sucesso!");
  } else {
    Serial.println("Erro ao criar fila");
    while(1);
  }
}

void loop() {
  // O loop fica vazio porque tudo é tratado pelas tarefas
  // Pode ser usado para debug ou tarefas de baixa prioridade
  vTaskDelay(pdMS_TO_TICKS(1000)); // Libera a CPU por 1 segundo
}