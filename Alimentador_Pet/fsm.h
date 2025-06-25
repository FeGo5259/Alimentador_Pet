#ifndef FSM_H
#define FSM_H
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

enum Evento {
  EVENTO_NENHUM = 0,
  EVENTO_UMIDADE_ALTA,
  EVENTO_UMIDADE_NORMAL,
  EVENTO_NIVEL_BAIXO,
  EVENTO_NIVEL_NORMAL,
  EVENTO_BOTAO_MANUAL,
  EVENTO_HORARIO,
  EVENTO_PORCAO_LIBERADA
};

enum Acao {
  NENHUMA_ACAO,
  A01_LED_UMIDADE_ON,
  A02_LED_UMIDADE_OFF,
  A03_LED_NIVEL_ON,
  A04_LED_NIVEL_OFF,
  A05_LIGAR_MOTOR,
  A06_DESLIGAR_MOTOR
};

enum Estado {
  STANDBY,
  ALERTA_UMIDADE,
  ALERTA_NIVEL,
  ALERTA_NIVEL_UMIDADE,
  LIBERANDO_PORCAO
};

extern QueueHandle_t xQueue;
extern Estado estadoAtual;
extern TickType_t tempoInicioLiberacao;
extern const TickType_t duracaoLiberacao;

void inicializarMaquinaEstados();
void taskMaquinaEstados(void *pvParameters);
void executarAcao(int acao);
void enviarMensagem(const String &msg);
#endif
