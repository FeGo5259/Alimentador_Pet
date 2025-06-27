#ifndef HORARIOS_H
#define HORARIOS_H
#include <Arduino.h>
extern int numHorarios;
extern QueueHandle_t xQueue;
void setupHora();
String obterHoraAtual();
void taskVerificaHorario(void* parameter);

#endif
