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
  // for (slave) id 1: write the value of 0x01, to the holding register at address 0x00 
  if (!ModbusRTUClient.holdingRegisterWrite(0x01, 0x00)) {
    SerialUSB.print("Failed to write holding register! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  }
  else{
    SerialUSB.println("Write successful for address 00");
  }
  // wait for 1 second
  delay(1000);

  // for (slave) id 1: write the value of 0x01, to the holding register at address 0x00 
  if (!ModbusRTUClient.holdingRegisterWrite(0x01, 0x01)) {
    SerialUSB.print("Failed to write holding register! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  }
  else{
    SerialUSB.println("Write successful for address 01");
  }
  // wait for 1 second
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 0x00, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp1 = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 00: ");
    SerialUSB.println(temp1);
    SerialUSB.print("Read successful from address 01: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 second
  delay(1000);
}
