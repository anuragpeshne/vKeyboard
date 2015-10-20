#include <stdio.h>
#include "kbdEmulator.h"
#include "receiver.h"

int getEventCode(char *);
int getKey(char *);

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
        emulateKey(getKey(buffer), getEventCode(buffer), fd);
      } else {
        break;
      }
    }
    destroyDevice(fd);
    return 0;
}

int getKey(char *buffer) {
  char tempBuf[10];
  int i = 0;
  while (buffer[i] != '-') {
    tempBuf[i] = buffer[i];
    i++;
  }
  tempBuf[i] = '\0';
  return atoi(tempBuf);
}

int getEventCode(char *buffer) {
  char tempBuf[10];
  int i = 0, j;

  while (buffer[i] != '-') i++;

  j = 0;
  while (buffer[i] != '\0') {
    tempBuf[j++] = buffer[i++];
  }
  tempBuf[j] = '\0';
  return atoi(tempBuf);
}
