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

#define PORT	 8080
#define MAXLINE 1024

#define GetBit(var, nbit) ((var & (1 << nbit)) != 0) // Returns true / false if bit is set
#define ResetBit(var, nbit) (var &= ~(1 << nbit));
#define SetBit(var, nbit) (var |= (1 << nbit))
#define FlipBit(var, nbit) (var ^= (1 << nbit))


int sockfd;
struct sockaddr_in servaddr;

class drDC{
	public:
	struct sockaddr_in alamat, cli_addr;
	// struct timeval t1, t2;
	long long enc;
	char buffer[MAXLINE];
	char terima[MAXLINE];
	char terimaSpesifik[MAXLINE];
	int sizeReceive;
	//lowpass
	float vFilt, vPrev;

	std::string deviceIP, senderIP;
	socklen_t len;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	drDC(char *IP, unsigned long port){
		memset(&alamat,/* code */ 0, sizeof(alamat));
		memset(&cli_addr, 0, sizeof(cli_addr));
		deviceIP = IP;
		drDC::alamat.sin_family = AF_INET;
		inet_pton(AF_INET, IP, &alamat.sin_addr.s_addr);
		alamat.sin_port = htons(port);
		len = sizeof(alamat);
		
	}

	/* Send UDP */
	void send(int soket, std::string pesan){
		sendto(soket, pesan.c_str(), strlen(pesan.c_str()),
			MSG_CONFIRM, (const struct sockaddr *) &alamat,
				len);
	}
	void drive(int cps){
		send(sockfd, std::to_string(cps));
	}
	/* Receive UDP */
	void receive(int soket){
		do{

			socklen_t sender_addr_len = sizeof(cli_addr);
			sizeReceive = recvfrom(soket, (char *)buffer, MAXLINE,
						0, ( struct sockaddr *) &cli_addr,
						&sender_addr_len);
			buffer[sizeReceive] = '\0';
			senderIP = inet_ntoa(cli_addr.sin_addr);
		}
		while(senderIP != deviceIP);

		strncpy(terima, buffer, MAXLINE);
        enc = atoi(terima);
	}
    operator long int (){
		receive(sockfd);
        return(atoi(terima));
    }

};

/*
	DAQ v2
	INPUT DIGITAL 8 pin: pin 1 - 8
	INPUT ANALOG 8 pin: pin 1 - 8

	OUTPUT DIGITAL 8 pin: pin 1 - 8
	OUTPUT ANALOG 8 pin: pin 1 -8
*/
class DAQ{
	private:
	std::thread* t;
	public:
	struct sockaddr_in device_addr;
	// struct timeval t1, t2;
	
	uint8_t terima[12];
	uint8_t kirim[12];
	int sizeReceive;

	
	
	socklen_t len;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	DAQ(char *IP, unsigned long port){
		memset(&device_addr, 0, sizeof(device_addr));
		
		DAQ::device_addr.sin_family = AF_INET;
		inet_pton(AF_INET, IP, &device_addr.sin_addr.s_addr);
		device_addr.sin_port = htons(port);
		len = sizeof(device_addr);
		t = new std::thread([this]() {update();} );
	}
	
	/* Send UDP */ //send 2 byte
	void update(){
		while(1){
			// std::this_thread::sleep_for(std::chrono::milliseconds(1));
			send(sockfd, kirim);
			receive(sockfd);
		}
	}
	void send(int soket, const uint8_t *pesan){
		sendto(soket, pesan, sizeof(pesan)+1,
			MSG_CONFIRM, (const struct sockaddr *) &device_addr,
				len);
	}
	/* Receive UDP */
	void receive(int soket){
		uint8_t buffer[12];
		struct sockaddr_in sender_addr;
		socklen_t sender_addr_len = sizeof(sender_addr);
		sizeReceive = recvfrom(soket, (uint8_t *)buffer, sizeof(buffer),
					0, ( struct sockaddr *) &sender_addr,
					&sender_addr_len);
		while(1){
			if(sender_addr.sin_addr.s_addr == device_addr.sin_addr.s_addr){
				memcpy(terima, buffer, sizeof(buffer));
				break;
			}
		}
	}

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
	void showBin(){
		for(uint8_t i = 0; i < 8; i++){
			printf("%d", GetBit(terima[0], i));
		}
		printf(" ");
		for(uint8_t i = 0; i < 8; i++){
			printf("%d ",terima[i+1]);
		}
		printf("\n");
	}
};

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
}

