#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "iostream"
#include "time.h"
#include "string"
#include "cmath"
#include "chrono"
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <thread>
#include <set>

#define PORT	 8080
#define MAXLINE 1024

#define GetBit(var, nbit) ((var & (1 << nbit)) != 0) // Returns true / false if bit is set
#define ResetBit(var, nbit) (var &= ~(1 << nbit));
#define SetBit(var, nbit) (var |= (1 << nbit))
#define FlipBit(var, nbit) (var ^= (1 << nbit))


int sockfd;
struct sockaddr_in servaddr;

uint8_t buffer[12];
struct sockaddr_in sender_addr;
socklen_t sender_addr_len = sizeof(sender_addr);
size_t sizeTerima;

/*
	DAQ v2
	INPUT DIGITAL 8 pin: pin 1 - 8
	INPUT ANALOG 8 pin: pin 1 - 8

	OUTPUT DIGITAL 8 pin: pin 1 - 8
	OUTPUT ANALOG 8 pin: pin 1 -8
*/


class DAQ{
	public:
	struct sockaddr_in device_addr;
	// struct timeval t1, t2;
	
	uint8_t terima[12];
	uint8_t kirim[12];
	int sizeReceive;
	unsigned long penghubung;
	
	socklen_t len;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	DAQ(char *IP, unsigned long port){
		memset(&device_addr, 0, sizeof(device_addr));
		DAQ::device_addr.sin_family = AF_INET;
		inet_pton(AF_INET, IP, &device_addr.sin_addr.s_addr);
		device_addr.sin_port = htons(port);
		len = sizeof(device_addr);
		penghubung = port;
		Register(this);
	}

	static DAQ* head;
	DAQ* next;
	static void Register(DAQ* obj){
		obj->next = head;
		head = obj;
	}
	void send(int soket, const uint8_t *pesan){
		sendto(soket, pesan, sizeof(pesan)+1,
			MSG_CONFIRM, (const struct sockaddr *) &device_addr,
				len);
	}

	void update(){
		// printf("%d\n",penghubung);
		send(sockfd, kirim);
		if(sender_addr.sin_addr.s_addr == this->device_addr.sin_addr.s_addr){
			memcpy(terima, buffer, sizeTerima);
		}
	}
	
	static void callonAllobj(){
		DAQ* current = head;
		while(current){
			current->update();
			current = current->next;
		}
	}
	/* Receive UDP */
    bool digitalRead(uint8_t pin){
        // receive(sockfd);
		if(GetBit(DAQ::terima[0], pin-1)){
			return 1;
		}
		else return 0;
    }

	uint8_t analogRead(uint8_t pin){
		// receive(sockfd);
		return DAQ::terima[pin];
	}

    void digitalWrite(uint8_t pin, bool STATE){
		STATE ? SetBit(kirim[0], pin-1) : ResetBit(kirim[0], pin-1);
		// send(sockfd, kirim);
    }

	void analogWrite(uint8_t pin, uint8_t PWM){
		kirim[pin] = PWM;
		// send(sockfd, kirim);
	}
	void showBinInput(){
		for(uint8_t i = 0; i < 8; i++){
			printf("%d", GetBit(terima[0], i));
		}
		printf(" ");
		for(uint8_t i = 0; i < 8; i++){
			printf("%d ",terima[i+1]);
		}
		printf("\n");
	}
	void showBinOutput(){
		for(uint8_t i = 0; i < 8; i++){
			printf("%d", GetBit(kirim[0], i));
		}
		printf(" ");
		for(uint8_t i = 0; i < 8; i++){
			printf("%d ",kirim[i+1]);
		}
		printf("\n");
	}
};

DAQ* DAQ::head = nullptr;

void communicate(int soket){
	// while(1){
		sizeTerima =  recvfrom(soket, (uint8_t *)buffer, sizeof(buffer),
						0, ( struct sockaddr *) &sender_addr,
						&sender_addr_len);
		DAQ::callonAllobj();
	// }
}

void startUDP(int udpPort){
    // Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(udpPort);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	// std::thread thread_listen(communicate, sockfd);
}

