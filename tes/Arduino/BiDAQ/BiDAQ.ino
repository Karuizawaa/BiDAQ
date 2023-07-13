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

#define IN1 22
#define IN2 23
#define IN3 24
#define IN4 25
#define IN5 26
#define IN6 27
#define IN7 28
#define IN8 29
#define IN9 30
#define IN10 31
#define IN11 32
#define IN12 33
#define IN13 34
#define IN14 35
#define IN15 36
#define IN16 37
#define IN17 38
#define IN18 39
#define IN19 40
#define IN20 41
#define IN21 42
#define IN22 43
#define IN23 44
#define IN24 45

const int IN[24] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};

#define OUT1 2
#define OUT2 3
#define OUT3 4
#define OUT4 5
#define OUT5 6
#define OUT6 7
#define OUT7 8
#define OUT8 9
#define OUT9 11
#define OUT10 12
#define OUT11 13

const int OUT[11] = {2,3,4,5,6,7,8,9,11,12,13};

// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// change the IP address, subnet mask, gateway's IP address, and DNS server's IP address depending on your network
IPAddress ip(192, 168, 0, 99);           //Setting IP Address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress myDns(8, 8, 8, 8);

//configure udp socket
EthernetUDP Udp;
unsigned int localPort = 8888;


char packetBuffer[512];  // buffer to hold incoming packet,


struct gy25 {
  char buffer[50];
  int counter;
  float heading;
  int headInt;
} cmps;

void updateIn(){
  String kirimBuffer;
  for(int i = 0; i < 23; i++){
    kirimBuffer += String(digitalRead(IN[i]));
  }
  Udp.print(kirimBuffer);
  Udp.endPacket();
}

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
  pinMode(IN11, INPUT);
  pinMode(IN12, INPUT);
  pinMode(IN13, INPUT);
  pinMode(IN14, INPUT);
  pinMode(IN15, INPUT);
  pinMode(IN16, INPUT);
  pinMode(IN17, INPUT);
  pinMode(IN18, INPUT);
  pinMode(IN19, INPUT);
  pinMode(IN20, INPUT);
  pinMode(IN21, INPUT);
  pinMode(IN22, INPUT);
  pinMode(IN23, INPUT);
  pinMode(IN24, INPUT);

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
  // initialize the Ethernet shield using the static IP address:
  Ethernet.begin(mac, ip, myDns, gateway, subnet);

  // initialize Udp communication on port
  Udp.begin(localPort);

}
void loop(){
  if(int n = Udp.parsePacket()){
    Udp.read(packetBuffer,50);  // buffer to hold incoming packet,
    packetBuffer[n] = '\0';
    for(int i = 0; i <= 50; i++){
      packetBuffer[i] == '0' ? digitalWrite(OUT[i], 0) : digitalWrite(OUT[i], 1); 
    }
  }
  updateIn();
}
