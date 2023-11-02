#include <BiDAQv3tes.h>

DAQ testing("192.168.0.99",8888);

int main(int argc, char const *argv[])
{
    startUDP(5555);
    testing.digitalWrite(2,1);
    while(1){
        // testing.digitalWrite(2,1);
        // communicate(sockfd);
        usleep(1000);
        // testing.digitalWrite(2,0);
        communicate(sockfd);
        // printf("wkwk\n");
        testing.showBinInput();
    }
    // for(int i = 0; i <10; i++){
    //     std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //     communicate(sockfd);
    //     std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //     std::cout << "Time to transmit + receive : " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
    // }
    return 0;
}

