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
short vHighGrid = 0, vLowGrid = 0, freqHighGrid = 0;
short freqLowGrid = 0, pfGrid = 0, socSystem = 0;
short powerBatSystem = 0, zeroExportPowerSystem = 0, timeBatSystem = 0;
short voltSystem = 0, docBatSystem = 0, batShutdownSystem = 0;
short voltageLoad = 0, currentSystem = 0, powerLoad = 0;
short batStartSystem = 0, currentLoad = 0;
int dataFlag = 0;
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
    PC_IF.print(F("This minimalistic demo will try to "));
    PC_IF.print(F("do the OTAA procedure and "));
    PC_IF.print(F("send a demo message each 30 sec.\n"));
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

void printPayload(uint8_t* buf, uint8_t size) {
  for (int i = 0; i < size; i++) {
      PC_IF.print((uint8_t) buf[i], HEX);
      PC_IF.print(F(" "));
  }
  PC_IF.print(F("\n"));
}
/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

void loop()
{
  Serial1.flush();
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 6, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw voltage(high values.
    vHighGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 6 vHighGrid: ");
    SerialUSB.println(vHighGrid);
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
    // get the raw voltage (low) values.
    vLowGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 7 vLowGrid: ");
    SerialUSB.println(vLowGrid);
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
    // get the raw frequency(high) values.
    freqHighGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 8 freqHighGrid: ");
    SerialUSB.println(freqHighGrid);
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
    // get the raw frequency(low) values.
    freqLowGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 9 freqLowGrid: ");
    SerialUSB.println(freqLowGrid);
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
    // get the raw power factor from grid values.
    pfGrid = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 10 pfGrid: ");
    SerialUSB.println(pfGrid);
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
    // get the raw SOC(state of charge of battery) values.
    socSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 11 socSystem: ");
    SerialUSB.println(socSystem);
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
    // get the raw Power of battery values.
    powerBatSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 12 powerBatSystem: ");
    SerialUSB.println(powerBatSystem);
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
    // get the raw zero export power (almost 20 w) values.
    zeroExportPowerSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 13 zeroExportPowerSystem: ");
    SerialUSB.println(zeroExportPowerSystem);
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
    // get the raw time of use of battery.
    timeBatSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 14 timeBatSystem: ");
    SerialUSB.println(timeBatSystem);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 15, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw voltage syem values.
    voltSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 15 voltSystem: ");
    SerialUSB.println(voltSystem);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 16, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw DOC of battery(almost 20%) values.
    docBatSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 16 docBatSystem: ");
    SerialUSB.println(docBatSystem);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 17, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw battery shutdown (almost 10%) values.
    batShutdownSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 17 batShutdownSystem: ");
    SerialUSB.println(batShutdownSystem);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 18, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw battery start (30%) values.
    batStartSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 18 batStartSystem: ");
    SerialUSB.println(batStartSystem);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 19, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw battery start (30%) values.
    currentSystem = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 19 currentSystem: ");
    SerialUSB.println(currentSystem);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 20, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw power values.
    powerLoad = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 20 powerLoad: ");
    SerialUSB.println(powerLoad);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 21, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw voltage values.
    voltageLoad = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 21 voltageLoad: ");
    SerialUSB.println(voltageLoad);
  }
  // wait for 1 milisecond
  delay(100);

  // send a Holding registers read request to (slave) id 1, for 1 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 22, 1)) {
    SerialUSB.print("failed to read registers! ");
    SerialUSB.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw current values.
    currentLoad = ModbusRTUClient.read();
    SerialUSB.print("Read successful from address 22 currentLoad: ");
    SerialUSB.println(currentLoad);
  }
  // wait for 1 milisecond
  delay(100);
      // send out a hello world every 30 sec ( =6* 50*100 ms)
      // (due to duty cycle restrictions 30 sec is recommended
      //if ((loopCnt > 1) && (loopCnt % (6*50)) == 0) {
          String id1 = "1";
          String id2 = "2";
          String id3 = "3";
          String id4 = "4";
          String id2 = "5";
          String grid = id1 + "_" + String(vHighGrid) + "_" + String(vLowGrid) + "_" + String(freqHighGrid)
          + "_" + String(freqLowGrid) + "_" + String(pfGrid);
          String system1 = id2 + "_" + String(socSystem) +"_"+ String(powerBatSystem) +"_"+ String(zeroExportPowerSystem);
          String system2 = id3 + "_" + String(timeBatSystem) +"_"+ String(voltSystem)+"_"+ String(docBatSystem);
          String system3 = id4 + "_" + String(batShutdownSystem) +"_"+ String(batStartSystem) +"_"+ String(currentSystem);
          String load = id5 + "_" + String(powerLoad) +"_"+ String(voltageLoad)+"_"+ String(currentLoad);
          
          // send out data
          PC_IF.print(F("Sending meter message...\n"));
          PC_IF.print(data1 + "\n");
          PC_IF.print(data2 + "\n");
          int test = 100;

          messageCnt++;
          
          // prepare TX data structure
          txData.Port = 0x42;
          //txData.Length = strlen_P(PSTR("Hello World!")); //insert simulated data
          //strcpy_P((char*) txData.Payload, PSTR("Hello World!"));
          if (dataFlag == 0) {
            const int dataSize = grid.length() + 1;  // Add 1 for null terminator
            char dataCharArray[dataSize];
            strcpy(dataCharArray, grid.c_str());  // Copy string to character array
            uint8_t* dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
            txData.Length = strlen_P((const char*)dataArray);
            strcpy_P((char*)txData.Payload, (const char*)dataArray);
            dataFlag = 1;
          }
          if (dataFlag == 1) {
            const int dataSize = system1.length() + 1;  // Add 1 for null terminator
            char dataCharArray[dataSize];
            strcpy(dataCharArray, system1.c_str());  // Copy string to character array
            uint8_t* dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
            txData.Length = strlen_P((const char*)dataArray);
            strcpy_P((char*)txData.Payload, (const char*)dataArray);
            dataFlag = 2;
          }
          if (dataFlag == 2) {
            const int dataSize = system2.length() + 1;  // Add 1 for null terminator
            char dataCharArray[dataSize];
            strcpy(dataCharArray, system2.c_str());  // Copy string to character array
            uint8_t* dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
            txData.Length = strlen_P((const char*)dataArray);
            strcpy_P((char*)txData.Payload, (const char*)dataArray);
            dataFlag = 3;
          }
          if (dataFlag == 3) {
            const int dataSize = system3.length() + 1;  // Add 1 for null terminator
            char dataCharArray[dataSize];
            strcpy(dataCharArray, system3.c_str());  // Copy string to character array
            uint8_t* dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
            txData.Length = strlen_P((const char*)dataArray);
            strcpy_P((char*)txData.Payload, (const char*)dataArray);
            dataFlag = 4;
          }
          else{
            const int dataSize = load.length() + 1;  // Add 1 for null terminator
            char dataCharArray[dataSize];
            strcpy(dataCharArray, load.c_str());  // Copy string to character array
            uint8_t* dataArray = reinterpret_cast<uint8_t*>(dataCharArray);  // Cast to uint8_t array
            txData.Length = strlen_P((const char*)dataArray);
            strcpy_P((char*)txData.Payload, (const char*)dataArray);
            dataFlag = 0;
          }
          
          //txData.Length = strlen_P(dataArray);
          //strcpy_P((char*) txData.Payload, txData.Length);
          //txData.Length = sizeof(data);
          //strcpy_P((char*) txData.Payload, sizeof(data));
          //printPayload(txData.Payload, txData.Length);
  
          // try to send a message
          if (false == wimod.SendUData(&txData)) {
              // an error occurred
  
              switch (wimod.GetLastResponseStatus()) {
                /* This case enforces the Duty cycle
                  case GLOBALLINK24_STATUS_CHANNEL_BLOCKED:
                      // duty cycle violation
                      // -> try again later
                      PC_IF.print(F("-> TX failed: Blocked due to DutyCycle...\r\n"));
                      break;
                */
                  case GLOBALLINK24_STATUS_DEVICE_NOT_ACTIVATED:
                      // activation problem
                      // -> try again later
                      PC_IF.print(F("-> TX failed: Device is still connecting to server...\r\n"));
                      break;
                  default:
                      PC_IF.print(F("-> TX failed\r\n"));
                      break;
              }
  
               // check if we have got a duty cycle problem
  
          } else {
              PC_IF.print(F("-> TX Done\r\n"));
          }
      //}

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(30000);
    loopCnt++;
}
