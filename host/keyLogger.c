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

int main(int argc, char *argv[]) {
  char *dev;
  struct input_event ev;
  ssize_t n;
  int fd;
  int sockfd;
  char buffer[50];

  if (argc < 3) {
    printf("Usage a.out {Hostname} {kbd device}");
    exit(1);
  }
  fd = open(argv[2], O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
    return EXIT_FAILURE;
  }
  sockfd = connectRemote(argv[1]);
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
      printf("put %s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
      memset(buffer, 0, 50);
      sprintf(buffer, "%d-%d", (int)ev.code, ev.value);
      sendMessage(sockfd, buffer);
    }
  }
  fflush(stdout);
  closeConnection(sockfd);
  fprintf(stderr, "%s.\n", strerror(errno));
  return EXIT_FAILURE;
}
