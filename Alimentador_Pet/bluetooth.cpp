#include "bluetooth.h"
#include "BluetoothSerial.h"
#include "motor.h"
#include "fsm.h" // Incluir fsm.h para acessar eventos e a fila xQueue
extern String horariosRecebidos[10];
extern int numHorarios;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void processarComandoBluetooth(const String &comando) {
    if (comando == "FEED") {
        int evento = EVENTO_BOTAO_MANUAL; // Define o evento manual
        xQueueSend(xQueue, &evento, 0);   // Envia o evento para a fila da FSM
    } else if (comando.startsWith("HOR:")) {
        SerialBT.println("Horário Recebido");
        int separador = comando.indexOf(";");
        horariosRecebidos[numHorarios++] = comando.substring(4, separador);
        int j = 0;
        for (int i = 0; i<numHorarios;i++){
            
            SerialBT.println("Horários de refeição: " +horariosRecebidos[j]);
            j++;
         
    } 
    }else if (comando.startsWith("QTD:")) {
        SerialBT.println("Quantidade Recebida");
        int porcao;
        String qtdStr = comando.substring(comando.indexOf("QTD:") + 4, comando.length() - 1);
        porcao = qtdStr.toInt();
        setPorcao(porcao);
        SerialBT.println("Porção Setada");
     
     }
    }



void bluetoothTask(void *param) {
    for (;;) {
        if (SerialBT.available()) {
            String recebido = SerialBT.readStringUntil('\n'); // Lê até encontrar quebra de linha
            recebido.trim(); // Remove espaços e quebras de linha extras
            if(recebido.length() > 0) {
                Serial.print("Recebido via BT: ");
                Serial.println(recebido);   
                processarComandoBluetooth(recebido);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Reduza o delay para resposta mais rápida
    }
}

void enviarMensagem(const String &msg) {
    SerialBT.println(msg); // ou outro método
}
