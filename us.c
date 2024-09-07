/*
port_no -> argv[1];	// message to be sent on this port
system_id -> argv[2];	// only use to print system id in that message
messgae -> argv[3]l
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFLEN 2048

int writeInSentFile(char *sys_id, char *port, char *message,char *senderId,char *senderPortId) ;
int main(int argc, char *argv[]) {
    int sockfd, reuse = 1;
    struct sockaddr_in receiverAddr;

    if (argc != 6) {
        fprintf(stderr, "Usage: %s <port_no> <system_id> <message> <sender_portId> <sender_sysId>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

   writeInSentFile(argv[2], argv[1], argv[3],argv[4],argv[5]);
    
    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(atoi(argv[1]));
    receiverAddr.sin_addr.s_addr = INADDR_ANY;
    char buffer[2048];
    sprintf(buffer, "sent by %s(%s) -> %s\n", argv[5],argv[4], argv[3]);
    //printf("Message Sent to port %s: %s\n", argv[1], buffer);
    if (sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &receiverAddr, sizeof(receiverAddr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Message Sent to port %s: %s\n", argv[1], argv[3]);
    close(sockfd);
    return 0;
}

int writeInSentFile(char *sys_id, char *port, char *message,char *senderId,char *senderPortId) {
    char fileName[512]; 	// size 512 <= sysid[256] + filename
    sprintf(fileName,"sentmsg_%s.txt",senderId);
    FILE *sent_msgfp = fopen(fileName, "a");
    if (!sent_msgfp) {
        perror("fopen error in creating file to store sent message  ");
        return 1;
    }
	char buffer[2048];
    sprintf(buffer, "%s(%s) to  %s(%s) -> %s\n",senderId,senderPortId,sys_id, port, message);
    fputs(buffer, sent_msgfp);
    
    fclose(sent_msgfp);
    return 0;
}
