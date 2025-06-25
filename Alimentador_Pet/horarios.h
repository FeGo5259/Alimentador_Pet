#ifndef HORARIOS_H
#define HORARIOS_H
#include <Arduino.h>
void inicializarHorarios();
bool verificarHorarioAtual(uint8_t hora, uint8_t minuto);
void taskVerificaHorario(void *pvParameters);

#endif
