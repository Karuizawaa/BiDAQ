# BiDAQ

to initiate communication, use startUDP function
e.x :
startUDP(5555);

*note 5555 is udp port for PC

for DAQ product, declare your DAQ with DAQ class.
e.x:

DAQ myDAQ("192.168.0.99, 8888);

DAQ myDAQ(urDAQ_IP, DAQPORT);

to make digital output, use DAQ::digitalWrite() method
e.x :

myDAQ.digitalWrite(3, 1);

*note
3, is digital output pin number
1, is state which is only contain 0 or 1

to make PWM output, use DAQ::analogWrite() method
e.x :

myDAQ.analogWrite(2, 150);

*note
2, is analog output pin number
150, is PWM value, which is only in range 0 to 255

to get digital input value, use DAQ::digitalRead() method
e.x:

myDAQ.digitalRead(3);

*note
3 is digital input pin number
