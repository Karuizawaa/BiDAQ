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

#define pinB3 8

uint16_t input;


// INPUT
uint8_t pin_input[10] = {2, 46, 3, 44, 18, 42, 19, 41, 31, 33};
#define IN1 2
#define IN2 46
#define IN3 3
#define IN4 44
#define IN5 18
#define IN6 42
#define IN7 19
#define IN8 41
#define IN9 31 
#define IN10 33

#define INMaxPin 16
uint16_t dataInputPin;

//OUT
uint8_t pin_output[10] = {13,12,11,9,3,25,23,4,27,29};
#define OUT1 13
#define OUT2 12
#define OUT3 11
#define OUT4 9
#define OUT5 3
#define OUT6  25
#define OUT7 23
#define OUT8 4
#define OUT9  27
#define OUT10 29

#define OUTMaxPin 16
uint16_t dataOutputPin;

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
uint16_t terima;

char headStr[20];

void setup() {
  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  pinMode(IN3, INPUT);
  pinMode(IN4, INPUT);
  pinMode(IN5, INPUT);
  pinMode(IN6, INPUT);
  pinMode(IN7, INPUT);
  pinMode(IN8, INPUT);
  pinMode(IN9, INPUT);
  pinMode(IN10, INPUT);

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
  // initialize the Ethernet shield using the static IP address:
  Ethernet.begin(mac, ip, myDns, gateway, subnet);

  // initialize Udp communication on port
  Udp.begin(localPort);
}

void loop() {
  if(int n = Udp.parsePacket()){
    Udp.read(packetBuffer,6);  // buffer to hold incoming packet,
    packetBuffer[n] = '\0';
    uint16_t outputVal = (packetBuffer[1] << 8 | packetBuffer[2]);
    for(uint8_t i=0; i <= OUTMaxPin; i++){
      digitalWrite(pin_output[i], outputVal & (1<<i));
    }
  }
  for(uint8_t i = 0; i <= INMaxPin-1; i++){
    if(digitalRead(pin_input) == 1){
      input |= 1 << i;
    }
    else{
      input &= ~(1 << i);
    }
  }
  Udp.write(input);
}
