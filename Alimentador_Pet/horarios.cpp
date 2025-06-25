#include "horarios.h"
#include <Arduino.h>
#include "fsm.h"


#define MAX_HORARIOS 5

struct Horario {
  uint8_t hora;
  uint8_t minuto;
};

Horario horarios[MAX_HORARIOS];
int totalHorarios = 0;

void inicializarHorarios() {
  // Exemplo fixo (pode ser configurado por Bluetooth depois)
  horarios[0] = {8, 30};
  horarios[1] = {12, 0};
  horarios[2] = {18, 30};
  totalHorarios = 3;
}

bool verificarHorarioAtual(uint8_t hora, uint8_t minuto) {
  for (int i = 0; i < totalHorarios; i++) {
    if (horarios[i].hora == hora && horarios[i].minuto == minuto) {
      return true;
    }
  }
  return false;
}

void taskVerificaHorario(void *pvParameters) {
  for (;;) {
    time_t now = millis() / 1000;
    uint8_t horaAtual = (now / 3600) % 24;
    uint8_t minutoAtual = (now / 60) % 60;

    if (verificarHorarioAtual(horaAtual, minutoAtual)) {
      int evento = 6; // EVENTO_HORARIO
      xQueueSend(xQueue, &evento, 0);
    }
    vTaskDelay(pdMS_TO_TICKS(60000));
  }
}
