#include <BiDAQv3.h>
DAQ test("192.168.0.99", 8888);
int main(int argc, char const *argv[])
{
    test.digitalWrite(2,1);
    
    return 0;
}
