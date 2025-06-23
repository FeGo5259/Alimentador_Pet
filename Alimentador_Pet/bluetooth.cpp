#include "bluetooth.h"
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void inicializarBluetooth() {
  SerialBT.begin("PetFeederESP32");
}

void enviarMensagem(const char* mensagem) {
  SerialBT.println(mensagem);
}

void taskBluetooth(void *pvParameters) {
  for (;;) {
    if (SerialBT.available()) {
      String comando = SerialBT.readStringUntil('\n');
      // Aqui pode-se tratar comandos como "ADD_HORARIO 08:30"
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
