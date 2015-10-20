#include <stdio.h>
#include "kbdEmulator.h"
#include "receiver.h"

int main(void) {
    int fd;
    int ret;
    char buffer[256];

    fd = createDevice(fd);
    printf("device created\n");
    ret = pollMsg(buffer);
    if (ret == 0) {
      emulateKey(atoi(buffer), fd);
      sleep(1);
    }

    destroyDevice(fd);
    return 0;
}

