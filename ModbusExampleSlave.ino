#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>


/* This example code has 9 holding registers. 6 analogue inputs, 1 button, 1 digital output
   and 1 register to indicate errors encountered since started.
   Function 5 (write single coil) is not implemented so I'm using a whole register
   and function 16 to set the onboard Led on the Atmega328P.
   
   The modbus_update() method updates the holdingRegs register array and checks communication.

   Note:  
   The Arduino serial ring buffer is 128 bytes or 64 registers.
   Most of the time you will connect the arduino to a master via serial
   using a MAX485 or similar.
 
   In a function 3 request the master will attempt to read from your
   slave and since 5 bytes is already used for ID, FUNCTION, NO OF BYTES
   and two BYTES CRC the master can only request 122 bytes or 61 registers.
 
   In a function 16 request the master will attempt to write to your 
   slave and since a 9 bytes is already used for ID, FUNCTION, ADDRESS, 
   NO OF REGISTERS, NO OF BYTES and two BYTES CRC the master can only write
   118 bytes or 59 registers.
 
   Using the FTDI USB to Serial converter the maximum bytes you can send is limited 
   to its internal buffer which is 60 bytes or 30 unsigned int registers. 
 
   Thus:
 
   In a function 3 request the master will attempt to read from your
   slave and since 5 bytes is already used for ID, FUNCTION, NO OF BYTES
   and two BYTES CRC the master can only request 54 bytes or 27 registers.
 
   In a function 16 request the master will attempt to write to your 
   slave and since a 9 bytes is already used for ID, FUNCTION, ADDRESS, 
   NO OF REGISTERS, NO OF BYTES and two BYTES CRC the master can only write
   50 bytes or 25 registers.
 
   Since it is assumed that you will mostly use the Arduino to connect to a 
   master without using a USB to Serial converter the internal buffer is set
   the same as the Arduino Serial ring buffer which is 128 bytes.
*/
 

// Using the enum instruction allows for an easy method for adding and 
// removing registers. Doing it this way saves you #defining the size 
// of your slaves register array each time you want to add more registers
// and at a glimpse informs you of your slaves register layout.

//////////////// registers of your slave ///////////////////
enum 
{     
  // just add or remove registers and your good to go...
  // The first register starts at address 0
  PWM_0,
  LED_ON_D7,
  ADC0,
  ADC1,
  ADC2,
  TOTAL_ERRORS,
  // leave this one
  TOTAL_REGS_SIZE = 100
  // total number of registers for function 3 and 16 share the same register array
};

unsigned int holdingRegs[TOTAL_REGS_SIZE]; // function 3 and 16 register array
////////////////////////////////////////////////////////////

#define RX            10     // Arduino defined pin (PB0, package pin #5)
#define TX            11     // Arduino defined pin (PB1, package pin #6)
#define RS485_EN      6     // pin to set transmission mode on RS485 chip (PB2, package pin #7)
#define BAUD_RATE     9600  // baud rate for serial communication
#define deviceID      1     // this device address

// SoftwareSerial mySerial(receive pin, transmit pin)
SoftwareSerial rs485(RX, TX); 

int v1p1 = 100;
int v1p2 = 100;
int v2p1 = 100;
int v2p2 = 100;
int v3p1 = 100;
int v3p2 = 100;
int a1p1 = 100;
int a1p2 = 100;
int a2p1 = 100;
int a2p2 = 100;
int a3p1 = 100;
int a3p2 = 100;
int p1p1 = 100;
int p1p2 = 100;
int p2p1 = 100;
int p2p2 = 100;
int p3p1 = 100;
int p3p2 = 100;
int k1p1 = 100;
int k1p2 = 100;
int k2p1 = 100;
int k2p2 = 100;
int k3p1 = 100;
int k3p2 = 100;

void setup()
{
  /* parameters(
                SoftwareSerial* comPort
                long baudrate, 
                unsigned char ID, 
                unsigned char transmit enable pin, 
                unsigned int holding registers size)
                
     The transmit enable pin is used in half duplex communication to activate a MAX485 or similar
     to deactivate this mode use any value < 2 because 0 & 1 is reserved for Rx & Tx
  */
  const int TOTAL_REGS_SIZE = 5000;
  modbus_configure(&rs485, BAUD_RATE, deviceID, RS485_EN, TOTAL_REGS_SIZE);
  pinMode(ADC1, INPUT);
  pinMode(ADC2, INPUT);
  pinMode(7,OUTPUT);
  holdingRegs[6] = v1p1;
  holdingRegs[7] = v1p2;
  holdingRegs[8] = v2p1;
  holdingRegs[9] = v2p2;
  holdingRegs[10] = v3p1;
  holdingRegs[11] = v3p2;
  holdingRegs[12] = a1p1;
  holdingRegs[13] = a1p2;
  holdingRegs[14] = a2p1;
  holdingRegs[15] = a2p2;
  holdingRegs[16] = a3p1;
  holdingRegs[17] = a3p2;
  holdingRegs[18] = p1p1;
  holdingRegs[19] = p1p2;
  holdingRegs[20] = p2p1;
  holdingRegs[21] = p2p2;
  holdingRegs[22] = p3p1;
  holdingRegs[23] = p3p2;
  holdingRegs[24] = k1p1;
  holdingRegs[25] = k1p2;
  holdingRegs[26] = k2p1;
  holdingRegs[27] = k2p2;
  holdingRegs[28] = k3p1;
  holdingRegs[29] = k3p2;
  Serial.begin(9600);

}
extern unsigned int errorCount;
void loop()
{
  // modbus_update() is the only method used in loop(). It returns the total error
  // count since the slave started. You don't have to use it but it's useful
  // for fault finding by the modbus master.
  holdingRegs[TOTAL_ERRORS] = modbus_update(holdingRegs);
  if (holdingRegs[1]==1)
  {
    digitalWrite(7,HIGH);
  }
  else
  {
    digitalWrite(7,LOW);
  }
  holdingRegs[ADC0]=analogRead(0);
  delay(100);
  //Serial.println(errorCount);
}
