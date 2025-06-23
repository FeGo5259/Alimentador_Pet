#ifndef BLUETOOTH_H
#define BLUETOOTH_H

void inicializarBluetooth();
void enviarMensagem(const char* mensagem);
void taskBluetooth(void *pvParameters);

#endif
