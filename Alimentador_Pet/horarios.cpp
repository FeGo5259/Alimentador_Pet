#include "horarios.h"
#include <Arduino.h>
#include "motor.h"
#include "fsm.h"
#include <esp_system.h>
#include <time.h>
#include <sys/time.h>

struct tm data;
String horariosRecebidos[10];
String now[10];
int numHorarios = 0;

// Simula hora atual fixa para testes
void setupHora(){
    timeval tv;
    tv.tv_sec = 1750982400;
    settimeofday(&tv, NULL);
}
String obterHoraAtual() {
    time_t tt = time(NULL);
    data = *gmtime(&tt);
    char data_formatada[64];
    strftime(data_formatada, 64, "%H%M", &data);
    return String(data_formatada);
}

void taskVerificaHorario(void* parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        String horaAtual = obterHoraAtual();
        //String horaAtual="2000";
        for (int i = 0; i < numHorarios; i++) {
            if (horaAtual == horariosRecebidos[i]) {
                int evento = EVENTO_HORARIO;
                xQueueSend(xQueue, &evento, 0);
                break;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(60000));
    }
}