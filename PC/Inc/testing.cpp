#include <BiDAQv3tes.h>

DAQ testing("192.168.0.99",8888);
int main(int argc, char const *argv[])
{
    startUDP(5555);
    testing.digitalWrite(2,1);
    while(1){
        // uint8_t buffer[12];
        // struct sockaddr_in sender_addr;
        // socklen_t sender_addr_len = sizeof(sender_addr);
        // size_t sizeTerima = recvfrom(sockfd, (uint8_t *)buffer, sizeof(buffer),
        //                     0, ( struct sockaddr *) &sender_addr,
        //                     &sender_addr_len);
                            
        // if(sender_addr.sin_addr.s_addr == testing.device_addr.sin_addr.s_addr){
        //     memcpy(testing.terima, buffer, sizeTerima);
        // }

        // communicate(sockfd);
        // testing.send(sockfd, testing.kirim);
        // for(auto& wkwk : DAQ::DAQSet()){
		// 	wkwk->showBinInput();
		// }

        testing.showBinInput();
        // DAQ::callonAllobj();

    }
    // for(int i = 0; i <10; i++){
    //     std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //     communicate(sockfd);
    //     std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //     printf("Time to transmit + receive : %d ns\n", std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count());
    // }
    return 0;
}

