#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "udp-client.h"
#include "udp-echo-server.h"
#include "tcp-client.h"
#include "tcp-echo-server.h"

#define MIN_PORT 1000
#define MAX_PORT 60000
#define MIN_LENGTH 32
#define MAX_LENGTH 65300
#define MIN_COUNT 1
#define MAX_COUNT 1000
#define MIN_INTERVAL 1
#define MAX_INTERVAL 10000
#define MIN_TIMEOUT 1
#define MAX_TIMEOUT 10000

int main(int argc, char** argv) {

	char* ip_addr = NULL;     //ip address
	int port = 5055;          //port
	int csflag = -1;          //client/server
	int msg_length = 256;     //message length bytes
	int msg_count = 1;        //n messages to send
	int msg_interval = 1000;  //delay interval
	int msg_timeout = 1000;   //client recv timeout
	int udp = false;          //tcp/udp

	int opt;
	while ((opt = getopt(argc, argv, "csup:l:k:i:t:h:")) != -1) {
		switch(opt) {
			case 'c':
				csflag = 0;
				break;
			case 's':
				csflag = 1;
				break;
			case 'u':
				udp = true;
				break;
			case 'p': {
				int val = atoi(optarg);
				if (val >= MIN_PORT && val <= MAX_PORT) {
					port = val;
				} else {
					perror("WARNING: Invalid port number, using default");
				}
				break;
			}
			case 'l': {
				int val = atoi(optarg);
				if (val >= MIN_LENGTH && val <= MAX_LENGTH) {
					msg_length = val;
				} else {
					perror("WARNING: Invalid message length, using default");
				}
				break;
			}
			case 'k': {
				int val = atoi(optarg);
				if (val >= MIN_COUNT && val <= MAX_COUNT) {
					msg_count = val;
				} else {
					perror("WARNING: Invalid message count, using default");
				}
				break;
			}
			case 'i': {
				int val = atoi(optarg);
				if (val >= MIN_INTERVAL && val <= MAX_INTERVAL) {
					msg_interval = val;
				} else {
					perror("WARNING: Invalid message interval, using default");
				}
				break;
			}
			case 't': {
				int val = atoi(optarg);
				if (val >= MIN_TIMEOUT && val <= MAX_TIMEOUT) {
					msg_timeout = val;
				} else {
					perror("WARNING: Invalid message timeout, using default");
				}
				break;
			}
			case 'h': {
			    struct sockaddr_in sa;
				if(inet_pton(AF_INET, optarg, &(sa.sin_addr)) > 0) {
					ip_addr = optarg;
				} else {
					perror("ERROR: Invalid IPv4 address");
				}
				break;
			}
			default:
				break;
		}
	}

	printf("client/server=%d, port=%d, length=%d, count=%d, interval=%d, timeout=%d, udp=%d, IP=%s\n",
			csflag, port, msg_length, msg_count, msg_interval, msg_timeout, udp, ip_addr);

	if (csflag < 0) {
		perror("ERROR: must specify either -c or -s");
		exit(EXIT_FAILURE);
	}

	if (csflag == 0) {

		if (ip_addr == NULL) {
			perror("Valid IPv4 address '-h' is required for client");
			exit(EXIT_FAILURE);
		}

		if (udp) {
			udp_send_msg(ip_addr, port, msg_length, msg_count, msg_interval, msg_timeout);
		} else {
			tcp_send_msg(ip_addr, port, msg_length, msg_count, msg_interval, msg_timeout);
		}

	} else {

		if (udp) {
			udp_echo_server_start(port);
		} else {
			tcp_echo_server_start(port);
		}

	}

	return EXIT_SUCCESS;

}
