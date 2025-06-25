#include "bluetooth.h"
#include "BluetoothSerial.h"
#include "motor.h"

extern String horariosRecebidos[10];
extern int numHorarios;
extern int quantidadeRecebida;

BluetoothSerial SerialBT;

void setupBluetooth() {
    SerialBT.begin("AlimentadorPet");
}

void processarComandoBluetooth(const String& comando) {
    if (comando == "FEED;") {
        taskMotor(*quantidadeRecebida);
    } else if (comando.startsWith("HOR:")) {
        int separador = comando.indexOf(";");
        String horariosStr = comando.substring(4, separador);
        String qtdStr = comando.substring(comando.indexOf("QTD:") + 4, comando.length() - 1);

        numHorarios = 0;
        int start = 0;
        while (start < horariosStr.length() && numHorarios < 10) {
            int end = horariosStr.indexOf(',', start);
            if (end == -1) end = horariosStr.length();
            horariosRecebidos[numHorarios++] = horariosStr.substring(start, end);
            start = end + 1;
        }

        quantidadeRecebida = qtdStr.toInt();
    }
}

void bluetoothTask(void *param) {
    for (;;) {
        if (SerialBT.available()) {
            String recebido = SerialBT.readStringUntil(';') + ";";
            processarComandoBluetooth(recebido);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void enviarMensagem(const String &msg) {
    SerialBT.println(msg); // ou outro método
}
