#include <WiMODGlobalLink24.h>
#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>
//-----------------------------------------------------------------------------
// platform defines
//-----------------------------------------------------------------------------
/*
 * Note: This sketch is for Arduino devices with two separate serial interfaces
 * (e.g. DUE). One interface is connected to a PC and one is used for WiMOD.
 *
 * For single serial interface boards (e.g. UNO) it is recommended to disbale
 * the PC / Debug messages
 */

#define WIMOD_IF    SerialWiMOD        // for Mega / Due use: Serial3
#define PC_IF    SerialUSB          // for Mega / Due use: Serial
#define RS845_DEFAULT_DE_PIN D6

// structure for storing the radio parameter set
TWiMODLR_DevMgmt_RadioConfig radioConfigSet;

static TWiMODGlobalLink24_TX_Data txData;

//-----------------------------------------------------------------------------
// constant values
//-----------------------------------------------------------------------------

/*
 * OTAA Parameters; must be changed by user
 */

// application  key (64bit)
const unsigned char APPEUI[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // CHANGE ME!!!

// application  key (128bit)
const unsigned char APPKEY[] = {0x53, 0x00, 0x6C, 0x1D, 0x8D, 0xF5, 0x46, 0x41, 0xA8,
                                0xB7, 0xFA, 0xE1, 0x9C, 0x1D, 0xCA, 0xE9 };      // CHANGE ME !!!

//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------

/*
 * Create in instance of the interface to the WiMOD-GlobalLink24 firmware
 */
WiMODGlobalLink24 wimod(WIMOD_IF);  // use the Arduino Serialx as serial interface

static uint32_t loopCnt = 0;
static uint32_t messageCnt = 0;

//-----------------------------------------------------------------------------
// data variables
//-----------------------------------------------------------------------------
// for slave id 1
short v1 = 0, v2 = 0, v3 = 0;
short a1 = 0, a2 = 0, a3 = 0;
short p1 = 0, p2 = 0, p3 = 0;
short k1 = 0, k2 = 0, k3 = 0;
short k1P2 = 0, k2P2 = 0, k3P2 = 0;
// for slave id 2
short vHighGrid = 0, vLowGrid = 0, freqHighGrid = 0;
short freqLowGrid = 0, pfGrid = 0, socSystem = 0;
short powerBatSystem = 0, zeroExportPowerSystem = 0, timeBatSystem = 0;
short voltSystem = 0, docBatSystem = 0, batShutdownSystem = 0;
short voltageLoad = 0, currentSystem = 0, powerLoad = 0;
short batStartSystem = 0, currentLoad = 0;
/*****************************************************************************
 * Function for printing out some debug infos via serial interface
 ****************************************************************************/
void debugMsg(String msg)
{
    PC_IF.print(msg);  // use default Arduino serial interface
}

void debugMsg(int a)
{
    PC_IF.print(a, DEC);
}

void debugMsgChar(char c)
{
    PC_IF.print(c);
}

void debugMsgHex(int a)
{
    if (a < 0x10) {
        PC_IF.print(F("0"));
    }
    PC_IF.print(a, HEX);
}
/*****************************************************************************
 * print out a welcome message
 ****************************************************************************/
void printStartMsg()
{
    PC_IF.print(F("==================================================\n"));
    PC_IF.print(F("This is FileName: "));
    PC_IF.print(F(__FILE__));
    PC_IF.print(F("\r\n"));
    PC_IF.print(F("Starting...\n"));
    PC_IF.print(F("This program will read data through modbus "));
    PC_IF.print(F("do the OTAA procedure and "));
    PC_IF.print(F("transmit the read messages every 30 sec.\n"));
    PC_IF.print(F("downlink messages will be printed in as hex values.\n"));
    PC_IF.print(F("==================================================\n"));
}
/*****************************************************************************
 * rx data callback for U Data
 ****************************************************************************/
void onRxUData(TWiMODLR_HCIMessage& rxMsg) {
    TWiMODGlobalLink24_RX_Data radioRxMsg;
    int i;

    debugMsg("Rx-U-Data Indication received.\n");

    // convert/copy the raw message to RX radio buffer
    if (wimod.convert(rxMsg, &radioRxMsg)) {

    if (radioRxMsg.StatusFormat & LORAWAN_FORMAT_ACK_RECEIVED) {
        // yes, this is an ack
        debugMsg(F("Ack-Packet received."));
    }
        // print out the received message as hex string
        if (radioRxMsg.Length > 0) {
            // print out the length
            debugMsg(F("Rx-U-Data Message: ["));
            debugMsg(radioRxMsg.Length);
            debugMsg(F("]: "));

            // print out the payload
            for (i = 0; i < radioRxMsg.Length; i++) {
                debugMsgHex(radioRxMsg.Payload[i]);
                debugMsg(F(" "));
            }
            debugMsg(F("\n"));
        }
    }
}
/*****************************************************************************
 * rx data callback for U Data
 ****************************************************************************/
void onRxCData(TWiMODLR_HCIMessage& rxMsg) {
    TWiMODGlobalLink24_RX_Data radioRxMsg;
    int i;

    debugMsg("Rx-C-Data Indication received.\n");

    // convert/copy the raw message to RX radio buffer
    if (wimod.convert(rxMsg, &radioRxMsg)) {

    if (radioRxMsg.StatusFormat & LORAWAN_FORMAT_ACK_RECEIVED) {
        // yes, this is an ack
        debugMsg(F("Ack-Packet received."));
    }
        // print out the received message as hex string
        if (radioRxMsg.Length > 0) {
            // print out the length
            debugMsg(F("Rx-C-Data Message: ["));
            debugMsg(radioRxMsg.Length);
            debugMsg(F("]: "));
            // print out the payload
            for (i = 0; i < radioRxMsg.Length; i++) {
                debugMsgHex(radioRxMsg.Payload[i]);
                debugMsg(F(" "));
            }
            debugMsg(F("\n"));
        }
    }
}

/*****************************************************************************
 * rx data callback
 ****************************************************************************/
void onRxAck(TWiMODLR_HCIMessage& rxMsg) {
    TWiMODGlobalLink24_RX_ACK_Data ackData;

    wimod.convert(rxMsg, &ackData);

    debugMsg(F("Received an ACK with status: "));
    debugMsg((int) ackData.StatusFormat);
    debugMsg(F("\n"));
}
/*****************************************************************************
 * no data indication callback
 ****************************************************************************/
void onNoData(void) {
    debugMsg(F("A No-Data Indication has been received "));
    debugMsg(F("--> (timeout for current (re)transmission)\n"));
}

void onTxUData(TWiMODLR_HCIMessage& rxMsg) {
    TWiMODGlobalLink24_TxIndData sendIndData;

    debugMsg(F("Tx-U-Data Indication: "));

    if (wimod.convert(rxMsg, &sendIndData)) {
        if (   (sendIndData.StatusFormat == LORAWAN_FORMAT_OK )
            || (sendIndData.StatusFormat == LORAWAN_FORMAT_EXT_HCI_OUT_ACTIVE))
        {
            debugMsg(F("TX OK"));
        } else {
            debugMsg(F("TX U-Data failed"));
        }
    }
    debugMsg(F("\n"));
}
/******************************************************************************
 tx indication callback for confirmed data
 *****************************************************************************/
void onTxCData(TWiMODLR_HCIMessage& rxMsg) {
    TWiMODGlobalLink24_TxIndData sendIndData;

    debugMsg(F("Tx-C-Data Indication: "));

    if (wimod.convert(rxMsg, &sendIndData)) {
        if (   (sendIndData.StatusFormat == LORAWAN_FORMAT_OK )
            || (sendIndData.StatusFormat == LORAWAN_FORMAT_EXT_HCI_OUT_ACTIVE))
        {
            debugMsg(F("TX OK"));

            if (sendIndData.FieldAvailability == LORAWAN_OPT_TX_IND_INFOS_INCL_PKT_CNT) {
                debugMsg(F(" ([re]try #:"));
                debugMsg(sendIndData.NumTxPackets);
                debugMsg(F(")"));
            }
        } else {
            debugMsg(F("TX C-Data failed"));
        }
    }
    debugMsg(F("\n"));
}

/*****************************************************************************
 * Arduino setup function
 ****************************************************************************/
void setup()
{
    // wait for the PC interface to be ready (max 10 sec); usefull for USB
    while (!PC_IF && millis() < 10000 ){}
    // debug interface
    PC_IF.begin(115200);

    SerialUSB.begin(9600);
    while (!SerialUSB);

    SerialUSB.println("Modbus RTU Client Toggle");
  
    // start the Modbus RTU client
    if (!ModbusRTUClient.begin(9600)) {
      SerialUSB.println("Failed to start Modbus RTU Client!");
      while (1);
    }

    // init / setup the serial interface connected to WiMOD
    WIMOD_IF.begin(WIMOD_GLOBALLINK24_SERIAL_BAUDRATE);

    // init the communication stack
    wimod.begin();

    printStartMsg();

    // print out some basic info about the module
    wimod.PrintBasicDeviceInfo(PC_IF);

    wimod.DeactivateDevice(); // for multiple starts

  //Change Class A to Class C
    TWiMODGlobalLink24_RadioStackConfig cfg;
        if ( wimod.GetRadioStackConfig(&cfg)) {
           cfg.Options = 0;
           cfg.Options = LORAWAN_STK_OPTION_ADR | LORAWAN_STK_OPTION_DEV_CLASS_C;
            wimod.SetRadioStackConfig(&cfg);
        }

    // register a client callbacks
    wimod.RegisterRxCDataIndicationClient(onRxCData);
    wimod.RegisterRxUDataIndicationClient(onRxUData);
    wimod.RegisterRxAckIndicationClient(onRxAck);
    wimod.RegisterNoDataIndicationClient(onNoData);
    wimod.RegisterTxCDataIndicationClient(onTxCData);
    wimod.RegisterTxUDataIndicationClient(onTxUData);
    
    // connect to a LoRaWAN server via an OTAA join
    wimod.ConnectViaOTAA(APPEUI, APPKEY);

}
/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

void loop()
{
  Serial1.flush();
  delay(30000);
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 6, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the values.
    v1 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 6 v1: ");
    SerialUSB.println(v1);
  }
  
  // wait for 1 milisecond
  delay(100);
  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 7, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    v2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 7 v2: ");
    SerialUSB.println(v2);
  }
  // wait for 1 milisecond
  delay(100);
  
  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 8, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    v3 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 8 v3: ");
    SerialUSB.println(v3);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 9, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    a1 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 9 a1: ");
    SerialUSB.println(a1);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 10, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    a2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 10 a2: ");
    SerialUSB.println(a2);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 11, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    a3 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 11 a3: ");
    SerialUSB.println(a3);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 12, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    p1 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 12 p1: ");
    SerialUSB.println(p1);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 13, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    p2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 13 p2: ");
    SerialUSB.println(p2);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 14, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    p3 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 14 p3: ");
    SerialUSB.println(p3);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 15, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    k1 = ModbusRTUClient.read();
    k1P2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 15 k1: ");
    SerialUSB.println(k1);
    SerialUSB.print("Read successful from address 16 k1: ");
    SerialUSB.println(k1P2);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 17, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    k2 = ModbusRTUClient.read();
    k2P2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 17 k2: ");
    SerialUSB.println(k2);
    SerialUSB.print("Read successful from address 18 k2: ");
    SerialUSB.println(k2P2);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 19, 2)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    k3 = ModbusRTUClient.read();
    k3P2 = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 19 k3: ");
    SerialUSB.println(k3);
    SerialUSB.print("Read successful from address 20 k3: ");
    SerialUSB.println(k3P2);
  }
  // wait for 1 milisecond
  delay(100);
  if (!ModbusRTUClient.requestFrom(2, HOLDING_REGISTERS, 30, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw voltage(high values.
    vHighGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 30 vHighGrid: ");
    SerialUSB.println(vHighGrid);
  }
  
  // wait for 1 milisecond
  delay(100);
  // send a Holding registers read request to (slave) id 2, for 1 registers
  if (!ModbusRTUClient.requestFrom(2, HOLDING_REGISTERS, 31, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw voltage (low) values.
    vLowGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 31 vLowGrid: ");
    SerialUSB.println(vLowGrid);
  }
  // wait for 1 milisecond
  delay(100);
  
  // send a Holding registers read request to (slave) id 2, for 1 registers
  if (!ModbusRTUClient.requestFrom(2, HOLDING_REGISTERS, 32, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw frequency(high) values.
    freqHighGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 32 freqHighGrid: ");
    SerialUSB.println(freqHighGrid);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 2, for 1 registers
  if (!ModbusRTUClient.requestFrom(2, HOLDING_REGISTERS, 33, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw frequency(low) values.
    freqLowGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 33 freqLowGrid: ");
    SerialUSB.println(freqLowGrid);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 2, for 1 registers
  if (!ModbusRTUClient.requestFrom(2, HOLDING_REGISTERS, 34, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw power factor from grid values.
    pfGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 34 pfGrid: ");
    SerialUSB.println(pfGrid);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 2, for 1 registers
  if (!ModbusRTUClient.requestFrom(2, HOLDING_REGISTERS, 35, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw power values.
    powerLoad = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 35 powerLoad: ");
    SerialUSB.println(powerLoad);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 2, for 1 registers
  if (!ModbusRTUClient.requestFrom(2, HOLDING_REGISTERS, 36, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw voltage values.
    voltageLoad = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 36 voltageLoad: ");
    SerialUSB.println(voltageLoad);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 2, for 1 registers
  if (!ModbusRTUClient.requestFrom(2, HOLDING_REGISTERS, 37, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw current values.
    currentLoad = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 37 currentLoad: ");
    SerialUSB.println(currentLoad);
  }
  // wait for 1 milisecond
  delay(100);
  // send out a hello world every 30 sec ( =6* 50*100 ms)
  // (due to duty cycle restrictions 30 sec is recommended
  //if ((loopCnt > 1) && (loopCnt % (6*50)) == 0) {
  String v = String(v1) + "_"+ String(v2) + "_" + String(v3);
  String a = String(a1) +"_"+ String(a2) +"_"+ String(a3);
  String p = String(p1) +"_"+ String(p2)+"_"+ String(p3);
  String k = String(k1) +"_"+ String(k2) +"_"+ String(k3);
  String grid = String(vHighGrid) +"_"+ String(vLowGrid) +"_"+ String(freqHighGrid) +"_"+ String(freqLowGrid)
  + "_" + String(pfGrid);
  String load = String(powerLoad) +"_"+ String(voltageLoad)+"_"+ String(currentLoad);
  String id1 = "1"; // for slave ID 1
  String id2 = "2"; // for slave ID 1
  String id3 = "3"; // for slave ID 2
  String id4 = "4"; // for slave ID 2
  String data1 = id1 +"_"+ String(v) +"_"+ String(a);
  String data2 = id2 +"_"+ String(p)+"_"+ String(k);
  String data3 = id3 +"_"+ String(grid);
  String data4 = id4 +"_"+ String(load);
  // send out data
  PC_IF.print(F("Sending meter message...\n"));
  PC_IF.print(data1 + "\n");
  PC_IF.print(data2 + "\n");
  PC_IF.print(data3 + "\n");
  PC_IF.print(data4);

  messageCnt++;
  
  // prepare TX data structure
  txData.Port = 0x42;
  
  const int dataSize1 = data1.length() + 1;  // Add 1 for null terminator
  char dataCharArray[dataSize1];
  strcpy(dataCharArray, data1.c_str());  // Copy string to character array
  uint8_t* dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
  txData.Length = strlen_P((const char*)dataArray);
  strcpy_P((char*)txData.Payload, (const char*)dataArray);
  // try to send a message data 1
  if (false == wimod.SendUData(&txData)) {
      // an error occurred
      switch (wimod.GetLastResponseStatus()) {
          case GLOBALLINK24_STATUS_DEVICE_NOT_ACTIVATED:
              // activation problem
              // -> try again later
              PC_IF.print(F("-> TX failed: Device is still connecting to server...\r\n"));
              break;
          default:
              PC_IF.print(F("-> TX failed\r\n"));
              break;
      }
      
  } else {
      PC_IF.print(F("-> TX Done\r\n"));
  }
  // check for any pending data of the WiMOD
  wimod.Process();
  delay(30000);
  
  const int dataSize2 = data2.length() + 1;  // Add 1 for null terminator
  dataCharArray[dataSize2];
  strcpy(dataCharArray, data2.c_str());  // Copy string to character array
  dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
  txData.Length = strlen_P((const char*)dataArray);
  strcpy_P((char*)txData.Payload, (const char*)dataArray);
  // try to send a message data 1
  if (false == wimod.SendUData(&txData)) {
      // an error occurred
      switch (wimod.GetLastResponseStatus()) {
          case GLOBALLINK24_STATUS_DEVICE_NOT_ACTIVATED:
              // activation problem
              // -> try again later
              PC_IF.print(F("-> TX failed: Device is still connecting to server...\r\n"));
              break;
          default:
              PC_IF.print(F("-> TX failed\r\n"));
              break;
      }
      
  } else {
      PC_IF.print(F("-> TX Done\r\n"));
  }
  // check for any pending data of the WiMOD
  wimod.Process();
  delay(30000);

  const int dataSize3 = data3.length() + 1;  // Add 1 for null terminator
  dataCharArray[dataSize3];
  strcpy(dataCharArray, data3.c_str());  // Copy string to character array
  dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
  txData.Length = strlen_P((const char*)dataArray);
  strcpy_P((char*)txData.Payload, (const char*)dataArray);
  // try to send a message data 1
  if (false == wimod.SendUData(&txData)) {
      // an error occurred
      switch (wimod.GetLastResponseStatus()) {
          case GLOBALLINK24_STATUS_DEVICE_NOT_ACTIVATED:
              // activation problem
              // -> try again later
              PC_IF.print(F("-> TX failed: Device is still connecting to server...\r\n"));
              break;
          default:
              PC_IF.print(F("-> TX failed\r\n"));
              break;
      }
      
  } else {
      PC_IF.print(F("-> TX Done\r\n"));
  }
  // check for any pending data of the WiMOD
  wimod.Process();
  delay(30000);

  const int dataSize4 = data4.length() + 1;  // Add 1 for null terminator
  dataCharArray[dataSize4];
  strcpy(dataCharArray, data4.c_str());  // Copy string to character array
  dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
  txData.Length = strlen_P((const char*)dataArray);
  strcpy_P((char*)txData.Payload, (const char*)dataArray);
  // try to send a message data 1
  if (false == wimod.SendUData(&txData)) {
      // an error occurred
      switch (wimod.GetLastResponseStatus()) {
          case GLOBALLINK24_STATUS_DEVICE_NOT_ACTIVATED:
              // activation problem
              // -> try again later
              PC_IF.print(F("-> TX failed: Device is still connecting to server...\r\n"));
              break;
          default:
              PC_IF.print(F("-> TX failed\r\n"));
              break;
      }
      
  } else {
      PC_IF.print(F("-> TX Done\r\n"));
  }

  // check for any pending data of the WiMOD
  wimod.Process();

  loopCnt++;
}
