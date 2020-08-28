#include "udp-echo-server.h"

//REF: https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c

void udp_echo_server_start(int port) {

	char buffer[BUFSIZE];
	struct sockaddr_in servaddr, cliaddr;
	int sockfd;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0) {
	    perror("ERROR: socket open failed");
	    exit(EXIT_FAILURE);
	}

	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
	    perror("ERROR: socket bind failed");
	    exit(EXIT_FAILURE);
	}

    printf("Listening on port %d...\n", port);

	while(true) {

		int client_len = sizeof(cliaddr);

		memset(&cliaddr, 0, client_len);
	    memset(buffer, 0, BUFSIZE);

	    int n = recvfrom(sockfd, buffer, BUFSIZE, 0, (struct sockaddr*) &cliaddr, (socklen_t*) &client_len);

	    if (n < 0) {
	    	perror("ERROR: receive failed");
		    exit(EXIT_FAILURE);
	    }

	    printf("Received %d bytes\n", n);

	    n = sendto(sockfd, buffer, n, 0, (struct sockaddr*) &cliaddr, client_len);

	    if (n < 0) {
	    	perror("ERROR: send failed");
		    exit(EXIT_FAILURE);
	    }

	}

}
