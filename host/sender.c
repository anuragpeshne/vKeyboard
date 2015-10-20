#include "sender.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int sendMessage(char *msg, char *hostname) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(PORT_NO);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    n = write(sockfd, msg, strlen(msg));
    if (n < 0)
      error("ERROR writing to socket");
    //memset(buffer, 0, 256);
    //n = read(sockfd,buffer,255);
    //if (n < 0)
         //error("ERROR reading from socket");
    //printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
