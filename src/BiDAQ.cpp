#include <BiDAQ.h>

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
		memset(&alamat, 0, sizeof(alamat));
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

class DAQ{
	public:
	struct sockaddr_in alamat, cli_addr;
	// struct timeval t1, t2;
	long long enc, lastenc, prevenc;
	char buffer[MAXLINE];
	char terima[MAXLINE];
	char terimaSpesifik[MAXLINE];
    char kirim[MAXLINE];
	int sizeReceive;
	//lowpass
	float vFilt, vPrev;

	std::string deviceIP, senderIP;
	socklen_t len;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	DAQ(char *IP, unsigned long port){
		memset(&alamat, 0, sizeof(alamat));
		memset(&cli_addr, 0, sizeof(cli_addr));
		deviceIP = IP;
		DAQ::alamat.sin_family = AF_INET;
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
	}

    bool digRead(uint8_t pin){
        receive(sockfd);
        char buff[20];
        if (pin != 0) buff[0] = terima[pin-1];
        return(atoi(buff));
    }

    void digWrite(uint8_t pin, bool STATE){
        STATE == 0 ? kirim[pin-1] = '0' : kirim[pin-1] = '1';
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