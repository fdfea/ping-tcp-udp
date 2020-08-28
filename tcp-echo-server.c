#include "tcp-echo-server.h"

//REF: http://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/www/class24code/echoserver.c

void tcp_echo_server_start(int port) {

	char buffer[BUFSIZE];
	struct sockaddr_in servaddr, cliaddr;
	int servfd, connfd;

	if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("ERROR: socket creation failed");
		exit(EXIT_FAILURE);
	}

    memset(&servaddr, 0, sizeof(servaddr));

    int optval = 1;
    setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

    if (bind(servfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0 ) {
        perror("ERROR: socket bind failed");
		exit(EXIT_FAILURE);
    }

    if (listen(servfd, 1) < 0) {
    	perror("ERROR: socket listen failed");
		exit(EXIT_FAILURE);
    }

    printf("Listening on port %d...\n", port);

    while(true) {

		memset(&cliaddr, 0, sizeof(cliaddr));

		int client_len = sizeof(cliaddr);

		connfd = accept(servfd, (struct sockaddr*) &cliaddr, (socklen_t*) &client_len);

		if (connfd < 0) {
			perror("ERROR: socket accept failed");
			exit(EXIT_FAILURE);
		}

		while(true) {

			memset(buffer, 0, BUFSIZE);

			int n = read(connfd, buffer, BUFSIZE);

			if (n < 0) {
				perror("ERROR: socket read failed");
				exit(EXIT_FAILURE);
			} else if (n == 0) {
				break;
			}

			printf("Received %d bytes\n", n);

			n = write(connfd, buffer, strlen(buffer));

			if (n < 0) {
			  perror("ERROR: socket write failed");
			  exit(EXIT_FAILURE);
			}

		}

	    close(connfd);

    }

}
