from pynput.keyboard import Key, Controller, KeyCode
import sys
import threading
import socket
import json


class Remote:
    KEY_PRESS = 0
    KEY_RELEASE = 1
    PORT = 8088

    @staticmethod
    def serve():
        serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        serversocket.bind(('localhost', Remote.PORT))
        serversocket.listen(5)
        while True:
            (clientsocket, address) = serversocket.accept()
            print(address)
            rem = Remote(clientsocket)
            rt = threading.Thread(target=rem.handle)
            rt.run()

    def __init__(self, sock):
        if len(sys.argv) >= 2 and sys.argv[1] == 'debug':
            self.debug = True
        else:
            self.debug = False
            self.keyboard = Controller()

        self.sock = sock

    def handle(self):
        while True:
            chunk = ''
            chunks = []
            while chunk != '\n':
                chunk = self.sock.recv(1)
                if chunk == '':
                    print("socket connection broken")
                    return
                chunks.append(chunk)

            msg = ''.join(chunks)
            try:
                msgObj = json.loads(msg.decode('UTF-8'))
            except json.JSONDecodeError:
                print("error in data")
                return

            if self.debug:
                print(msgObj)
            else:
                if "Key." in msgObj['key']:
                    key = getattr(KeyCode, msgObj['key'].split('.')[1])
                else:
                    key = msgObj['key']
                if msgObj['event'] == Remote.KEY_PRESS:
                    self.keyboard.press(key)
                elif msgObj['event'] == Remote.KEY_RELEASE:
                    self.keyboard.release(key)
                else:
                    print("Unknown Key event")

if __name__ == "__main__":
    Remote.serve()
