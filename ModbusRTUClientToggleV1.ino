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
  // for (slave) id 1: write the value of 0x05, to the holding register at address 0x00
  Serial1.flush();
  /*
  if (!ModbusRTUClient.holdingRegisterWrite(5, 0x00, 0x05)) {
    SerialUSB.print("Failed to write holding register! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  }
  else {
    SerialUSB.println("Write successful for address 00");
  }
  // wait for 1 second
  delay(1000);
  
  // for (slave) id 1: write the value of 0x01, to the holding register at address 0x01
  if (!ModbusRTUClient.holdingRegisterWrite(5, 0x01, 0x01)) {
    SerialUSB.print("Failed to write holding register! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  }
  else {
    SerialUSB.println("Write successful for address 01");
  }
  // wait for 1 second
  delay(1000);
  */
  // send a Holding registers read request to (slave) id 1, for 2 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 6, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 6 v1: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 7 v1: ");
    SerialUSB.println(temp2);
  }
  
  // wait for 1 milisecond
  delay(1000);
  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 8, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 8 v2: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 9 v2: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);
  
  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 10, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 10 v3: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 11 v3: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 12, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 12 a1: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 13 a1: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 14, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 14 a2: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 15 a2: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 16, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 16 a3: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 17 a3: ");
    SerialUSB.println(temp);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 18, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 18 p1: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 19 p1: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 20, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 20 p2: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 21 p2: ");
    SerialUSB.println(temp);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 22, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 22 p3: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 23 p3: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 24, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 24 k1: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 25 k1: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 26, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 26 k2: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 27 k2: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 28, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short temp = ModbusRTUClient.read();
    short temp2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 28 k3: ");
    SerialUSB.println(temp);
    SerialUSB.print("Read successful from address 29 k3: ");
    SerialUSB.println(temp2);
  }
  // wait for 1 milisecond
  delay(1000);
}
