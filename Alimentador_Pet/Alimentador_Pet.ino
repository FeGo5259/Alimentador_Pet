#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "fsm.h"
#include "sensores.h"
#include "motor.h"
#include "leds.h"
#include "bluetooth.h"
#include "horarios.h"

QueueHandle_t xQueue;

void setup() {
  Serial.begin(115200);

  inicializarSensores();
  inicializarLeds();
  inicializarBluetooth();
  inicializarHorarios();
  inicializarMaquinaEstados();

  xQueue = xQueueCreate(10, sizeof(int));
  if (xQueue != NULL) {
    xTaskCreate(taskMaquinaEstados, "FSM", 2048, NULL, 2, NULL);
    xTaskCreate(taskVerificaSensores, "Sensores", 2048, NULL, 1, NULL);
    xTaskCreate(taskBluetooth, "Bluetooth", 2048, NULL, 1, NULL);
    xTaskCreate(taskVerificaHorario, "Horario", 2048, NULL, 1, NULL);
    vTaskStartScheduler();
  } else {
    Serial.println("Erro ao criar fila");
  }
}

void loop() {}
