#include <stdio.h>
#include "kbdEmulator.h"
#include "receiver.h"

int main(void) {
    int fd, sockfd, newsockfd;
    int ret;
    char buffer[256];

    fd = createDevice(fd);
    printf("device created\n");
    connectHost(&sockfd, &newsockfd);
    while(1) {
      ret = pollMsg(newsockfd, buffer);
      if (ret == 0) {
              printf("%s", buffer);
        //emulateKey(atoi(buffer), fd);
      }
    }
    destroyDevice(fd);
    return 0;
}

