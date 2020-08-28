#include "tcp-client.h"

//REF: https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/tcpclient.c

void tcp_send_msg(char* ip_addr, int port, int msg_length, int msg_count, int msg_interval, int msg_timeout) {

    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFSIZE];
    int npkts_rx = 0;
    double tot_latency = 0.0, max_latency = 0.0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR: socket open failed");
        exit(EXIT_FAILURE);
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = msg_timeout*1000;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv, sizeof(tv));

    memset(&servaddr, 0, sizeof(servaddr));

    inet_aton(ip_addr, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;

    if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
    	perror("ERROR: server connection failed");
    	exit(EXIT_FAILURE);
    }

    int c;
    for(c = 0; c < msg_count; c++) {

		memset(buffer, 0, BUFSIZE);
		memset(buffer, 'A', msg_length);

		struct timespec start, end;
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        int n = write(sockfd, buffer, msg_length);

        if (n < 0) {
        	perror("ERROR: send message failed");
        	exit(EXIT_FAILURE);
        }

		memset(buffer, 0, BUFSIZE);

        n = read(sockfd, buffer, msg_length);

		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		double t_elapsed = ((double) (end.tv_sec - start.tv_sec) * 1e3) + ((double) (end.tv_nsec - start.tv_nsec) / 1e6);

		if (n < 0) {
			perror("ERROR: receive message failed");
		} else {
			npkts_rx += 1;
	        tot_latency += t_elapsed;
	        if(t_elapsed > max_latency) {
	        	max_latency = t_elapsed;
	        }
		}

        printf("Reply from %s: bytes=%d time=%.3fms\n", inet_ntoa(servaddr.sin_addr), n, t_elapsed);

        usleep(msg_interval*1000);

    }

    printf("%d pkts tx, %d pkts rx, %.2f%% loss\n", msg_count, npkts_rx, 1.0 - (((double) npkts_rx)/msg_count));
    printf("Latency: avg=%0.3fms, max=%0.3fms\n", tot_latency/msg_count, max_latency);

    close(sockfd);

}
