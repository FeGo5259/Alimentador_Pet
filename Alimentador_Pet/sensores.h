#ifndef SENSORES_H
#define SENSORES_H

void inicializarSensores();
bool umidadeAlto();     // retorna true se umidade > 50%
bool nivelBaixo();      // retorna true se nível estiver abaixo do ideal
void taskVerificaSensores(void *pvParameters);

#endif
