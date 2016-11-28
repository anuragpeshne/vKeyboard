from pynput.keyboard import Key, Listener

send = False

def on_press(key):
    global send
    if send and key != Key.caps_lock:
        print('{0} pressed'.format(key))

def on_release(key):
    global send
    if key == Key.esc:
        # Stop listener
        return False
    elif key == Key.caps_lock:
        send = not send
    if send and key != Key.caps_lock:
        print('{0} release'.format(key))


# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()
