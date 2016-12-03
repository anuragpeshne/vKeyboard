from pynput.keyboard import Key, Listener
import sys
import socket
import json

class Host:
    PORT = 8088
    KEY_PRESS = 0
    KEY_RELEASE = 1

    def __init__(self):
        if len(sys.argv) < 2:
            print('Pass remote addr')
            exit(1)
        elif len(sys.argv) > 2 and sys.argv[2] == 'debug':
            self.debug = True
        else:
            self.debug = False
            remoteAddr = sys.argv[1]

        if not self.debug:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((remoteAddr, Host.PORT))
        self.send = False

    def sendKey(self, key, eventType):
        key = str(key).encode('ascii')
        if len(key) == 4:
            key = key[2:3]
        print len(key)
        sendObj = json.dumps({'key': key, 'event': eventType})

        if self.debug:
            print sendObj
        else:
            sent = self.sock.send(sendObj + '\n')
            if sent == 0:
                raise RuntimeError("socket connection broken")

    def start(self):
        def on_press(key):
            if self.send and key != Key.caps_lock:
                self.sendKey(key, Host.KEY_PRESS)

        def on_release(key):
            if key == Key.esc:
                # Stop listener
                return False
            elif key == Key.caps_lock:
                self.send = not self.send
            if self.send and key != Key.caps_lock:
                self.sendKey(key, Host.KEY_RELEASE)

        # Collect events until released
        with Listener(
                on_press=on_press,
                on_release=on_release) as listener:
            listener.join()

if __name__ == "__main__":
    h = Host()
    h.start()
