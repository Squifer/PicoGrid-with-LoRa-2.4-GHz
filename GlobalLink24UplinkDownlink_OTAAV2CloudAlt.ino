/*
 * This is a minimalistic example file to show how to use the WiMOD Arduino
 * library to communicate with a WiMOD Module by IMST GmbH
 *
 * http://www.wireless-solutions.de
 *
 */

/*
 * Example:
 *
 * This example demonstrates how to send a 'Hello World' uplink message via
 * the WiMOD module to a server emulating LoRaWAN on 2.4 GHz
 * and receiving downlink messages from the server
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running WiMOD_GlobalLink24_EndNode_Modemfirmware
 *
 * Usage:
 * -------
 * - Change the keys according to your server before starting
 * - Start the program and watch the serial monitor @ 115200 baud
 */

// make sure to use only the WiMODGlobalLink24.h
#include <WiMODGlobalLink24.h>

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


// structure for storing the radio parameter set
TWiMODLR_DevMgmt_RadioConfig radioConfigSet;

//-----------------------------------------------------------------------------
// constant values
//-----------------------------------------------------------------------------
static TWiMODGlobalLink24_TX_Data txData;
/*
 * OTAA Parameters; must be changed by user
 */

// application  key (64bit)
const unsigned char APPEUI[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // CHANGE ME!!!

// application  key (128bit)
const unsigned char APPKEY[] = {0x2F, 0xB6, 0x21, 0x4D, 0x3D, 0xA2, 0xE1, 0x72, 0x6F,
                                0x43, 0x5A, 0x61, 0xFF, 0xD4, 0x07, 0xB5 };      // CHANGE ME !!!

//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------

/*
 * Create in instance of the interface to the WiMOD-GlobalLink24 firmware
 */
WiMODGlobalLink24 wimod(WIMOD_IF);  // use the Arduino Serialx as serial interface

static uint32_t loopCnt = 0;

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

    // send out a hello world every 30 sec ( =6* 50*100 ms)
    // (due to duty cycle restrictions 30 sec is recommended
    if ((loopCnt > 1) && (loopCnt % (6*50)) == 0) {
        // send out a simple HelloWorld messsage
        PC_IF.print(F("Sending HelloWorld message...\n"));

        // prepare TX data structure
        txData.Port = 0x42;
        txData.Length = strlen_P(PSTR("Hello World!"));
        strcpy_P((char*) txData.Payload, PSTR("Hello World!"));

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
    }

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(100);
    loopCnt++;
}
