#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFLEN 2048

int main(int argc, char *argv[]) {
    int sockfd, n;
    char buffer[BUFLEN];
    struct sockaddr_in receiverAddr, senderAddr;
    int reuse = 1;
    socklen_t len;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port_no>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR option
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    memset(&senderAddr, 0, sizeof(senderAddr));

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_addr.s_addr = INADDR_ANY;
    receiverAddr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (const struct sockaddr *) &receiverAddr, sizeof(receiverAddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    len = sizeof(senderAddr);
    while (1) {
        n = recvfrom(sockfd, (char *)buffer, BUFLEN, MSG_WAITALL, (struct sockaddr *) &senderAddr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';
	FILE *fp;
	char fname[512];
	sprintf(fname,"recvMessage_%s.txt",argv[1]);
	fp=fopen(fname,"a");
	if(fp == NULL){
		printf("Error in file opeening\n");
		exit(0);
	}
	fputs(buffer,fp);
	fclose(fp);
       // printf("Message received on port %s: %s\n", argv[1], buffer);
    }

    close(sockfd);
    return 0;
}

