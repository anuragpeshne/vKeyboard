#include "kbdEmulator.h"

//key codes are arranged as keyboard layout, this function converts ascii char
// to keyboard key and gives back KEY_CODE
int convertToKeyCode(char ip) {
  int rowIndex;
  if (ip == 'a') {
    return KEY_A;
  } else if (ip == 'b') {
    return KEY_B;
  } else if (ip == 'c') {
    return KEY_C;
  } else {
    return KEY_D;
  }
}

int createDevice(int fd) {
  struct uinput_user_dev uidev;
  int i;

  //open the character device in writeonly and non block mode
  fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  if(fd < 0)
    die("error: open");

  // inform input subsystem that we'll use key press and release events
  if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
    die("error: ioctl 27");
  if(ioctl(fd, UI_SET_EVBIT, EV_REP) < 0)
    die("error: ioctl 27");
  //synchronization events
  if(ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)
    die("error: ioctl 34");

  // key code for keys are defined in qwerty fashion; use loop to register them
  // all: from KEY_ESC key to KEY_F10
  for (i = KEY_ESC; i <= KEY_F10; i++) {
    if(ioctl(fd, UI_SET_KEYBIT, i) < 0)
      die("error: ioctl %d", i);
  }

  //register new input device
  memset(&uidev, 0, sizeof(uidev));
  snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
  uidev.id.bustype = BUS_USB;
  uidev.id.vendor  = 0x1;
  uidev.id.product = 0x1;
  uidev.id.version = 1;

  if(write(fd, &uidev, sizeof(uidev)) < 0)
    die("error: write");

  if(ioctl(fd, UI_DEV_CREATE) < 0)
    die("error: ioctl 49");

  return fd;
}

void emulateKey(int key, int fd) {
  struct input_event ev;

  //key press
  memset(&ev, 0, sizeof(struct input_event));
  ev.type = EV_KEY;
  ev.code = key;
  ev.value = 1;
  if(write(fd, &ev, sizeof(struct input_event)) < 0)
      die("error: write");

  memset(&ev, 0, sizeof(struct input_event));
  ev.type = EV_SYN;
  ev.code = SYN_REPORT;
  ev.value = 0;
  if(write(fd, &ev, sizeof(struct input_event)) < 0)
      die("error: write");

  //key release
  memset(&ev, 0, sizeof(struct input_event));
  ev.type = EV_KEY;
  ev.code = key;
  ev.value = 0;
  if(write(fd, &ev, sizeof(struct input_event)) < 0)
      die("error: write");

  memset(&ev, 0, sizeof(struct input_event));
  ev.type = EV_SYN;
  ev.code = SYN_REPORT;
  ev.value = 0;
  if(write(fd, &ev, sizeof(struct input_event)) < 0)
      die("error: write");

  usleep(1500);
}

void destroyDevice(fd) {
  if(ioctl(fd, UI_DEV_DESTROY) < 0)
    die("error: ioctl 59");

  close(fd);
}
