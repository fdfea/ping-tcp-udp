#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFSIZE 65300

void udp_send_msg(char* ip_addr, int port, int msg_length, int msg_count, int msg_interval, int msg_timeout);

#endif
