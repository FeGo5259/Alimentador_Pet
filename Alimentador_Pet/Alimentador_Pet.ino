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

QueueHandle_t xQueue;

#define MAX_HORARIOS 10

int quantidade = 0;
bool jaDisparado[MAX_HORARIOS];

String horariosRecebidos[10];
int numHorarios = 0;
int quantidadeRecebida = 0;

void verificarHorarioTask(void *param) {
    for (;;) {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Erro ao obter hora");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            continue;
        }

        char horaAtual[6];
        sprintf(horaAtual, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);

        for (int i = 0; i < numHorarios; i++) {
            if (horariosRecebidos[i] == horaAtual) {
                taskMotor(*quantidadeRecebida);
                vTaskDelay(60000 / portTICK_PERIOD_MS);  // espera 1 minuto pra não repetir
                break;
            }
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS);  // checa a cada 10s
    }
}

void setup() {
  Serial.begin(115200);

  setupMotor();
  inicializarSensores();
  inicializarLeds();
  setupBluetooth();
  inicializarHorarios();
  inicializarMaquinaEstados();

  xQueue = xQueueCreate(10, sizeof(int));
  if (xQueue != NULL) {
    xTaskCreate(taskMaquinaEstados, "FSM", 2048, NULL, 2, NULL);
    xTaskCreate(taskVerificaSensores, "Sensores", 2048, NULL, 1, NULL);
    xTaskCreate(bluetoothTask, "Bluetooth", 2048, NULL, 1, NULL);
    xTaskCreate(taskVerificaHorario, "Horario", 2048, NULL, 1, NULL);
    vTaskStartScheduler();
  } else {
    Serial.println("Erro ao criar fila");
  }
}

void loop() {}
