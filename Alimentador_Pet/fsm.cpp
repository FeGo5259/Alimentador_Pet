#include "fsm.h"
#include "leds.h"
#include "motor.h"
#include "bluetooth.h"

extern QueueHandle_t xQueue;
extern Estado estadoAtual;
extern TickType_t tempoInicioLiberacao;
extern const TickType_t duracaoLiberacao;

#define NUM_ESTADOS 5
#define NUM_EVENTOS 8
TickType_t tempoInicioLiberacao = 0;
const TickType_t duracaoLiberacao = pdMS_TO_TICKS(3000);

int matriz_acao[NUM_ESTADOS][NUM_EVENTOS];
int matriz_proximo_estado[NUM_ESTADOS][NUM_EVENTOS];

Estado estadoAtual = STANDBY;

void inicializarMaquinaEstados() {
  for (int i = 0; i < NUM_ESTADOS; i++) {
    for (int j = 0; j < NUM_EVENTOS; j++) {
      matriz_acao[i][j] = NENHUMA_ACAO;
      matriz_proximo_estado[i][j] = i;
    }
  }

  // STANDBY
  matriz_acao[STANDBY][EVENTO_UMIDADE_ALTA] = A01_LED_UMIDADE_ON;
  matriz_proximo_estado[STANDBY][EVENTO_UMIDADE_ALTA] = ALERTA_UMIDADE;

  matriz_acao[STANDBY][EVENTO_NIVEL_BAIXO] = A03_LED_NIVEL_ON;
  matriz_proximo_estado[STANDBY][EVENTO_NIVEL_BAIXO] = ALERTA_NIVEL;

  matriz_acao[STANDBY][EVENTO_BOTAO_MANUAL] = A05_LIGAR_MOTOR;
  matriz_proximo_estado[STANDBY][EVENTO_BOTAO_MANUAL] = LIBERANDO_PORCAO;

  matriz_acao[STANDBY][EVENTO_HORARIO] = A05_LIGAR_MOTOR;
  matriz_proximo_estado[STANDBY][EVENTO_HORARIO] = LIBERANDO_PORCAO;

  // ALERTA_UMIDADE
  matriz_acao[ALERTA_UMIDADE][EVENTO_NIVEL_BAIXO] = A03_LED_NIVEL_ON;
  matriz_proximo_estado[ALERTA_UMIDADE][EVENTO_NIVEL_BAIXO] = ALERTA_NIVEL_UMIDADE;

  matriz_acao[ALERTA_UMIDADE][EVENTO_UMIDADE_NORMAL] = A02_LED_UMIDADE_OFF;
  matriz_proximo_estado[ALERTA_UMIDADE][EVENTO_UMIDADE_NORMAL] = STANDBY;

  matriz_acao[ALERTA_UMIDADE][EVENTO_BOTAO_MANUAL] = A05_LIGAR_MOTOR;
  matriz_proximo_estado[ALERTA_UMIDADE][EVENTO_BOTAO_MANUAL] = LIBERANDO_PORCAO;

  matriz_acao[ALERTA_UMIDADE][EVENTO_HORARIO] = A05_LIGAR_MOTOR;
  matriz_proximo_estado[ALERTA_UMIDADE][EVENTO_HORARIO] = LIBERANDO_PORCAO;

  // ALERTA_NIVEL
  matriz_acao[ALERTA_NIVEL][EVENTO_UMIDADE_ALTA] = A01_LED_UMIDADE_ON;
  matriz_proximo_estado[ALERTA_NIVEL][EVENTO_UMIDADE_ALTA] = ALERTA_NIVEL_UMIDADE;

  matriz_acao[ALERTA_NIVEL][EVENTO_NIVEL_NORMAL] = A04_LED_NIVEL_OFF;
  matriz_proximo_estado[ALERTA_NIVEL][EVENTO_NIVEL_NORMAL] = STANDBY;

  matriz_acao[ALERTA_NIVEL][EVENTO_BOTAO_MANUAL] = A05_LIGAR_MOTOR;
  matriz_proximo_estado[ALERTA_NIVEL][EVENTO_BOTAO_MANUAL] = LIBERANDO_PORCAO;

  matriz_acao[ALERTA_NIVEL][EVENTO_HORARIO] = A05_LIGAR_MOTOR;
  matriz_proximo_estado[ALERTA_NIVEL][EVENTO_HORARIO] = LIBERANDO_PORCAO;

  // ALERTA_NIVEL_UMIDADE
  matriz_acao[ALERTA_NIVEL_UMIDADE][EVENTO_UMIDADE_NORMAL] = A02_LED_UMIDADE_OFF;
  matriz_proximo_estado[ALERTA_NIVEL_UMIDADE][EVENTO_UMIDADE_NORMAL] = ALERTA_NIVEL;

  matriz_acao[ALERTA_NIVEL_UMIDADE][EVENTO_NIVEL_NORMAL] = A04_LED_NIVEL_OFF;
  matriz_proximo_estado[ALERTA_NIVEL_UMIDADE][EVENTO_NIVEL_NORMAL] = ALERTA_UMIDADE;

  matriz_acao[ALERTA_NIVEL_UMIDADE][EVENTO_BOTAO_MANUAL] = A05_LIGAR_MOTOR;
  matriz_proximo_estado[ALERTA_NIVEL_UMIDADE][EVENTO_BOTAO_MANUAL] = LIBERANDO_PORCAO;

  matriz_acao[ALERTA_NIVEL_UMIDADE][EVENTO_HORARIO] = A05_LIGAR_MOTOR;
  matriz_proximo_estado[ALERTA_NIVEL_UMIDADE][EVENTO_HORARIO] = LIBERANDO_PORCAO;

  // LIBERANDO_PORCAO
  matriz_acao[LIBERANDO_PORCAO][EVENTO_PORCAO_LIBERADA] = A06_DESLIGAR_MOTOR;
  matriz_proximo_estado[LIBERANDO_PORCAO][EVENTO_PORCAO_LIBERADA] = STANDBY;
}

void executarAcao(int acao) {
  switch (acao) {
    case A01_LED_UMIDADE_ON:
      ledUmidadeOn();
      enviarMensagem("Alerta: Umidade alta");
      break;
    case A02_LED_UMIDADE_OFF:
      ledUmidadeOff();
      enviarMensagem("Umidade normalizada");
      break;
    case A03_LED_NIVEL_ON:
      ledNivelOn();
      enviarMensagem("Alerta: Nível baixo");
      break;
    case A04_LED_NIVEL_OFF:
      ledNivelOff();
      enviarMensagem("Nível normalizado");
      break;
    case A05_LIGAR_MOTOR:
      acionaMotor();
      enviarMensagem("Liberando Porção");
      break;
    case A06_DESLIGAR_MOTOR:
      desligarMotor();
      enviarMensagem("Porção liberada");
      break;
  }
}

void taskMaquinaEstados(void *pvParameters) {
  int evento;
  for (;;) {
    if (xQueueReceive(xQueue, &evento, portMAX_DELAY) == pdPASS) {
      int acao = matriz_acao[estadoAtual][evento];
      Estado proximo = (Estado)matriz_proximo_estado[estadoAtual][evento];

      executarAcao(acao);
      estadoAtual = proximo;
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}