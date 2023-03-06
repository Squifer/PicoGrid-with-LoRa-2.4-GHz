/*
  Modbus RTU Client Toggle

  This sketch toggles the coil of a Modbus RTU server connected via RS485
  on and off every second.

  Circuit:
   - MKR board
   - MKR 485 shield
     - ISO GND connected to GND of the Modbus RTU server
     - Y connected to A/Y of the Modbus RTU server
     - Z connected to B/Z of the Modbus RTU server
     - Jumper positions
       - FULL set to OFF
       - Z \/\/ Y set to ON

  created 16 July 2018
  by Sandeep Mistry
*/

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

#define RS845_DEFAULT_DE_PIN D6


void setup() {
  SerialUSB.begin(9600);
  while (!SerialUSB);

  SerialUSB.println("Modbus RTU Client Toggle");

  // start the Modbus RTU client
  if (!ModbusRTUClient.begin(9600)) {
    SerialUSB.println("Failed to start Modbus RTU Client!");
    while (1);
  }
}

void loop() {
  // for (slave) id 1: write the value of 0x01, to the coil at address 0x00 
  if (!ModbusRTUClient.holdingRegisterWrite(0x01, 0x01)) {
    SerialUSB.print("Failed to write holding register! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  }

  // wait for 1 second
  delay(1000);
  SerialUSB.println("Test");

  // for (slave) id 1: write the value of 0x00, to the coil at address 0x00 
  if (!ModbusRTUClient.coilWrite(1, 0x00, 0x00)) {
    SerialUSB.print("Failed to write coil! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  }

  // wait for 1 second
  delay(1000);
}
