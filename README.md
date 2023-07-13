# BiDAQ

to initiate communication, use startUDP function
e.x :
startUDP(5555);

*note 5555 is udp port for PC

for DAQ product, declare your DAQ with DAQ class.
e.x:

DAQ myDAQ(urDAQ_IP, DAQPORT);


for drDC product, declare your drDC with drDC class.
e.x:

drDC motor1(drDC_IP, drDCPORT);

DAQ made for read digital pulse and write digital pulse. this is how you can read digital pulse form the desired pin and write digital pulse to desired pin.

myDAQ.digRead(PIN);     //read digital pulse from PIN

myDAQ.digWrite(PIN);    //write digital pulse to PIN

drDC made for controlling motor DC from PC. this product can give you the encoder value, and you can control the motor speed with encoder count / second;
e.x:
motor1.drive(CPS);      //drive motor with speed count per second