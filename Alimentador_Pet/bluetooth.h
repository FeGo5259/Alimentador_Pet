#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <Arduino.h>

#include <BluetoothSerial.h>

extern BluetoothSerial SerialBT;  // <- apenas declara

void setupBluetooth();
void processarComandoBluetooth(const String& comando);
void bluetoothTask(void *param);
void enviarMensagem(const String &msg);

#endif
