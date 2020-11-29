# ping-tcp-udp

Simple ping program, using TCP or UDP, instead of ICMP, to measure the round trip time 
of a packet between a client and server. 

### Usage

Example command to compile the program:  
`gcc -Wall -O3 -o pingtcpudp *.c`

The program can run as a client or a server. The intended use is for one device to 
run the server and another to run the client so the two can communicate. 

The program takes several command line arguments:
* `-s`/`-c` \[required] -- run the program as the client or server
* `-h` \[required client] -- IPv4 address of server
* `-p` \[optional client/server] -- port to client/server on, default is 5055
* `-u` \[optional client] -- use udp, default is tcp
* `-k` \[optional client] -- number of packets to send, default is 1
* `-l` \[optional client] -- length of packets to send in bytes, default is 256
* `-i` \[optional client] -- interval to send packets in milliseconds, default is 1000
* `-t` \[optional client] -- timeout to wait for response in milliseconds, deafult is 1000

The arguments are provided by inputting the parameter *directly* after the argument flag, if the flag accepts a parameter. 

Example commands:  
./pingtcpudp -s  
./pingtcpudp -s -p12121
./pingtcpudp -c -h127.0.0.1  
./pingtcpudp -c -u -h10.0.0.2 -p5044 -l512 -k4 -i2000

The program will display the latency for each packet sent, and then the total number of packets 
sent/received and the average/max latency once all the packets have been sent. 

### Credits

Forrest Feaser ([@fdfea](https://github.com/fdfea))
