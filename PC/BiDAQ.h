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

#define PORT	 8080
#define MAXLINE 1024

int sockfd;
struct sockaddr_in servaddr;

class Device{
	public:
	struct sockaddr_in alamat, cli_addr;
	// struct timeval t1, t2;
	long long enc, lastenc, prevenc;
	char buffer[MAXLINE];
	char terima[MAXLINE];
	char terimaSpesifik[MAXLINE];
	int sizeReceive;
	float sum;
	float hadap;
	float radps;

	//lowpass
	float vFilt, vPrev;

	std::string deviceIP, senderIP;
	socklen_t len;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	Device(char *IP, unsigned long port){
		memset(&alamat, 0, sizeof(alamat));
		memset(&cli_addr, 0, sizeof(cli_addr));
		deviceIP = IP;
		Device::alamat.sin_family = AF_INET;
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
		
		if(senderIP == "192.168.0.99"){
			// strncpy(terima, buffer, MAXLINE);
			hadap = atof(strtok(terima+11, "~"));
		}
		else {
			enc = atoi(terima);
		}

	}

	/* PID P000]rocess alamat */
	int PID(float KP, float KI, float setradPS, float PULSEPERREV){
		begin = std::chrono::steady_clock::now();
		double nanosec = std::chrono::duration_cast<std::chrono::nanoseconds> (begin - end).count();
		radps = ((enc - lastenc) * 2 * M_PI / PULSEPERREV) / (nanosec*pow(10,-9));
		lastenc = enc;

		vFilt = 0.854 * vFilt + 0.0728 * radps + 0.0728 * vPrev;
		vPrev = radps;

		float error = setradPS - radps;
		sum += error;
		end = std::chrono::steady_clock::now();
		if(setradPS == 0){
			sum = 0;
			return 0;
		}
		else return(KP * error + KI * sum);
	}
};