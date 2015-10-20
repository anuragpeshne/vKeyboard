/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receiver.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int pollMsg(int newsockfd, char *buffer) {
     int n;
     memset(buffer, 0, 256);
     n = read(newsockfd,buffer,4);
     if (n < 0) error("ERROR reading from socket");
     //printf("Here is the message: %s\n",buffer);
     //n = write(newsockfd,"I got your message",18);
     //if (n < 0) error("ERROR writing to socket");
     return n;
}

void connectHost(int *sockfd, int *newsockfd) {
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     *sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (*sockfd < 0)
        error("ERROR opening socket");
     memset((char *) &serv_addr, 0, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORT_NO);
     if (bind(*sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) {
              //error("ERROR on binding");
              printf("skipping");
              return;
     }
     listen(*sockfd,5);
     clilen = sizeof(cli_addr);
     *newsockfd = accept(*sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (*newsockfd < 0)
          error("ERROR on accept");
}

void closeConnection(int sockfd, int newsockfd) {
     close(newsockfd);
     close(sockfd);
}
