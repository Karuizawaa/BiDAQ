  /*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-ethernet-shield-2
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// INPUT DIGITAL
uint8_t pin_input[16] = {31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 22, 24, 26, 28, 30, 32};
//PullUP
#define IN1 31
#define IN2 33
#define IN3 35
#define IN4 37
#define IN5 39
#define IN6 41
#define IN7 49
#define IN8 45
#define IN9 47 
#define IN10 49

#define IN11 22
#define IN12 24
#define IN13 26
#define IN14 28
#define IN15 30
#define IN16 32

#define INMaxPin 16
uint16_t dataInputPin;

//OUT
uint8_t pin_output[16] = {23, 3, 25, 27, 4, 29, 5, 6, 7, 8, 9, 11, 12, 13, 34, 36};
#define OUT1 23
#define OUT2 3
#define OUT3 25
#define OUT4 27
#define OUT5 4
#define OUT6 29
#define OUT7 5
#define OUT8 6
#define OUT9 7
#define OUT10 8
#define OUT11 9
#define OUT12 11
#define OUT13 12
#define OUT14 13
#define OUT15 34
#define OUT16 36

#define OUTMaxPin 16
uint16_t dataOutputPin;

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
char packetBuffer[6];  // buffer to hold incoming packet,
uint8_t terimaBuffer[6];
uint16_t terima;

char headStr[20];

void setup() {
  pinMode(IN1, INPUT_PULLUP);
  pinMode(IN2, INPUT_PULLUP);
  pinMode(IN3, INPUT_PULLUP);
  pinMode(IN4, INPUT_PULLUP);
  pinMode(IN5, INPUT_PULLUP);
  pinMode(IN6, INPUT_PULLUP);
  pinMode(IN7, INPUT_PULLUP);
  pinMode(IN8, INPUT_PULLUP);
  pinMode(IN9, INPUT_PULLUP);
  pinMode(IN10, INPUT_PULLUP);
  pinMode(IN11, INPUT);
  pinMode(IN12, INPUT);
  pinMode(IN13, INPUT);
  pinMode(IN14, INPUT);
  pinMode(IN15, INPUT);
  pinMode(IN16, INPUT);

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(OUT5, OUTPUT);
  pinMode(OUT6, OUTPUT);
  pinMode(OUT7, OUTPUT);
  pinMode(OUT8, OUTPUT);
  pinMode(OUT9, OUTPUT);
  pinMode(OUT10, OUTPUT);
  pinMode(OUT11, OUTPUT);
  pinMode(OUT12, OUTPUT);
  pinMode(OUT13, OUTPUT);
  pinMode(OUT14, OUTPUT);
  pinMode(OUT15, OUTPUT);
  pinMode(OUT16, OUTPUT);
  // initialize the Ethernet shield using the static IP address:
  Ethernet.begin(mac, ip, myDns, gateway, subnet);

  // initialize Udp communication on port
  Udp.begin(localPort);
  Serial.begin(115200);
}

void loop() {
  if(int n = Udp.parsePacket()){
    Udp.read(terimaBuffer,6);  // buffer to hold incoming packet,
    uint16_t outputVal = (packetBuffer[1] << 8 | packetBuffer[0]);
    packetBuffer[n] = 0;
//    for(uint8_t i=0; i <= OUTMaxPin - 1; i++){
////      digitalWrite(pin_output[i], GetBit(outputVal, i));
//      Serial.print(GetBit(outputVal, i));
//    }
    for(uint8_t i = 0 ; i < 8 ; i++){
      Serial.print(GetBit(terimaBuffer[0], i));
    }
    for(uint8_t i = 0 ; i < 8 ; i++){
      Serial.print(GetBit(terimaBuffer[1], i));
    }
    Serial.println();
  }
  for(uint8_t i = 0; i <= INMaxPin-1; i++){
    if(digitalRead(pin_input[i]) == 1){
      SetBit(dataInputPin, i);
    }
    else{
      ResetBit(dataInputPin, i);
    }
  }
  Udp.beginPacket(IPAddress(192, 168, 0, 55), 5555);
  Udp.write(dataInputPin);
  Udp.endPacket();
  delay(1);
}
