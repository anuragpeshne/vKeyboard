#include <stdio.h>
#include "kbdEmulator.h"
#include "receiver.h"

int main(void) {
    int fd, sockfd, newsockfd;
    int len;
    char buffer[256];

    fd = createDevice(fd);
    printf("device created\n");
    connectHost(&sockfd, &newsockfd);
    while(1) {
      len = pollMsg(newsockfd, buffer);
      if (len > 0) {
        emulateKey(atoi(buffer), fd);
      } else {
        break;
      }
    }
    destroyDevice(fd);
    return 0;
}

