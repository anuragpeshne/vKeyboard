#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include "sender.h"

static const char *const evval[3] = {
  "RELEASED",
  "PRESSED ",
  "REPEATED"
};

int main(void) {
  const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
  struct input_event ev;
  ssize_t n;
  int fd;
  int sockfd;
  char buffer[50];

  fd = open(dev, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
    return EXIT_FAILURE;
  }
  sockfd = connectRemote("10.100.3.123");
  while (1) {
    n = read(fd, &ev, sizeof ev);
    if (n == (ssize_t)-1) {
      if (errno == EINTR)
        continue;
      else
        break;
    } else {
      if (n != sizeof ev) {
        errno = EIO;
        break;
      }
    }
    if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2) {
      //printf("got %s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
      if (ev.value == 1) {
      	printf("put %s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
        memset(buffer, 0, 50);
        sprintf(buffer, "%d", (int)ev.code);
        sendMessage(sockfd, buffer);
      }
    }
  }
  fflush(stdout);
  closeConnection(sockfd);
  fprintf(stderr, "%s.\n", strerror(errno));
  return EXIT_FAILURE;
}
