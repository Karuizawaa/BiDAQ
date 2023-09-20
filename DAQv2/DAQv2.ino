/*
 * DAQ from Arduino MEGA
 * INPUT Digital 8 Pin
 * INPUT Analog 8 Pin
 * 
 * OUTPUT Digital 8 Pin
 * OUTPUT PWM 8 Pin
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

//INPUT ANALOG
uint8_t pin_inputA[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
#define INA1 A0
#define INA2 A1
#define INA3 A2
#define INA4 A3
#define INA5 A4
#define INA6 A5
#define INA7 A6
#define INA8 A7

// INPUT DIGITAL
uint8_t pin_inputD[8] = {31, 33, 35, 37, 39, 41, 43, 45};
#define IND1 31
#define IND2 33
#define IND3 35
#define IND4 37
#define IND5 39
#define IND6 41
#define IND7 49
#define IND8 45
uint8_t dataInputPin;


//OUTPUT PWM
uint8_t pin_outputP[8] = {2, 3, 4, 5, 6, 7, 8, 9};
#define OUTP1 2
#define OUTP2 3
#define OUTP3 4
#define OUTP4 5
#define OUTP5 6
#define OUTP6 7
#define OUTP7 8
#define OUTP8 9

//OUTPUT DIGITAL
uint8_t pin_outputD[8] = {30, 32, 34, 36, 38, 40, 42, 44};
#define OUTD1 30
#define OUTD2 32
#define OUTD3 34
#define OUTD4 36
#define OUTD5 38
#define OUTD6 40
#define OUTD7 42
#define OUTD8 44

#define GetBit(var, nbit) ((var & (1 << nbit)) != 0) // Returns true / false if bit is set
#define ResetBit(var, nbit) (var &= ~(1 << nbit));
#define SetBit(var, nbit) (var |= (1 << nbit))
#define FlipBit(var, nbit) (var ^= (1 << nbit))

// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// change the IP address, subnet mask, gateway's IP address, and DNS server's IP address depending on your network
IPAddress ip(192, 168, 0, 99);           //Setting IP Address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress myDns(8, 8, 8, 8);
EthernetUDP Udp;

unsigned int localPort = 8888;
uint8_t terimaBuffer[12];

char headStr[20];

void setup() {
  pinMode(IND1, INPUT);
  pinMode(IND2, INPUT);
  pinMode(IND3, INPUT);
  pinMode(IND4, INPUT);
  pinMode(IND5, INPUT);
  pinMode(IND6, INPUT);
  pinMode(IND7, INPUT);
  pinMode(IND8, INPUT);
  
  pinMode(INA1, INPUT);
  pinMode(INA2, INPUT);
  pinMode(INA3, INPUT);
  pinMode(INA4, INPUT);
  pinMode(INA5, INPUT);
  pinMode(INA6, INPUT);
  pinMode(INA7, INPUT);
  pinMode(INA8, INPUT);



  pinMode(OUTP1, OUTPUT);
  pinMode(OUTP2, OUTPUT);
  pinMode(OUTP3, OUTPUT);
  pinMode(OUTP4, OUTPUT);
  pinMode(OUTP5, OUTPUT);
  pinMode(OUTP6, OUTPUT);
  pinMode(OUTP7, OUTPUT);
  pinMode(OUTP8, OUTPUT);
  
  pinMode(OUTD1, OUTPUT);
  pinMode(OUTD2, OUTPUT);
  pinMode(OUTD3, OUTPUT);
  pinMode(OUTD4, OUTPUT);
  pinMode(OUTD5, OUTPUT);
  pinMode(OUTD6, OUTPUT);
  pinMode(OUTD7, OUTPUT);
  pinMode(OUTD8, OUTPUT);
  // initialize the Ethernet shield using the static IP address:
  Ethernet.begin(mac, ip, myDns, gateway, subnet);

  // initialize Udp communication on port
  Udp.begin(localPort);
  Serial.begin(115200);
}

void loop() {
  if(int n = Udp.parsePacket()){
    Udp.read(terimaBuffer,12);  // buffer to hold incoming packet,
    //BYTE PERTAMA DIGITAL OUTPUT, BYTE SELANJUTNYA PWM : 1 BYTE PERTAMA, 8 PIN DIGITALL, 8 BYTE SISA 8 PIN ANALOG
    for(uint8_t i = 0; i < 8; i++){
      digitalWrite(pin_outputD[i], GetBit(terimaBuffer[0], i));
      Serial.print(GetBit(terimaBuffer[0], i));
    }
    Serial.print(" ");
    for(uint8_t i = 0; i < 8; i++){
      analogWrite(pin_outputP[i], terimaBuffer[i+1]);
      Serial.print(terimaBuffer[i+1]);
      Serial.print(" ");
    }
    Serial.println();
  }
  for(uint8_t i = 0; i < 8; i++){
    if(digitalRead(pin_inputD[i]) == 1){
      SetBit(dataInputPin, i);
    }
    else{
      ResetBit(dataInputPin, i);
    }
  }
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(dataInputPin);  //Byte pertama info pin input digital
  for(uint8_t i = 0; i < 8 ; i++){
    uint16_t val = analogRead(pin_inputA[i]);
    val = map(val, 0, 1023, 0, 255);  //Ubah 10-bit jadi 8-bit
    Udp.write(val);         // Byte sisanya info pin input Analog
  }
  Udp.endPacket();
  delay(1);
}
