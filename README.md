# vKeyboard
Share keyboard across computers

###### Motivation:
I wrote this program to feed input to my Raspberry Pi using my laptop's keyboard over LAN. However, it can be used to provide input to any Linux device using keyboard connected to any Linux device over any network.

###### Caveat:
* This program is intended to be used on a trusted network. Keyboard keys are transported over plain TCP sockets and thus can be messed up with.
* Further, the program relies on `/dev/input/by-path/{kbd-device}` to read keyboard keys and `/dev/uinput` (user level input subsystem) to emulate keyboard events. Make sure they exist on your devices.

###### How To:
1. Clone.
2. On Remote (where keyboard input needs to be provided):
  1. cd into `/remote`
  2. `gcc *.c`
  3. `sudo ./a.out &`
  4. `ctrl-c` to stop.
3. On Host (where Keyboard is connected):
  1. cd into `/host`.
  2. `gcc *.c`
  3. `sudo ./a.out {IP address of remote} {/dev/input/by-path/name of keyboard file}`
  4. optional: preceed above command by `stty -echo` to prevent terminal from echoing back; execute `stty echo` to return to original behavior.
  5. `ctrl-c` to stop.

###### To-Do:
* Make key combinations to enable and disable sending key codes.
* Ignore `sigint` on host, or have a way around.
* Try to make it work on Android. (Android has `/dev/uinput`.
* Write keyboard reader for OS X.
