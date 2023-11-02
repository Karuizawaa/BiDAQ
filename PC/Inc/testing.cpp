#include <BiDAQv3tes.h>

DAQ testing("192.168.0.99",8888);
int main(int argc, char const *argv[])
{
    startUDP(5555);
    
    for(int i =0; i <10; i++){
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        communicate(sockfd);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time to transmit + receive : " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
        testing.digitalWrite(2, testing.digitalRead(1));
    }
    return 0;
}

