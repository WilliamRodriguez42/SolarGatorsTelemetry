import RPi.GPIO as GPIO
import time
import threading

GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.IN)
current = 0
previous = 0
beginning = time.time()
running = True
thread = None
speed = 0
hist_length = 10
history = [beginning] * hist_length

def histor():
    global history, speed
    
    history.append(time.time())
    del history[0]
    
    dtime = history[-1] - history[0]
    speed = hist_length/dtime
    
def check():
    global current,previous,beginning,summ
    current = GPIO.input(4)
    if(current == 0 and previous == 1):
        histor()
        

    previous = current

def startThread():
    global running,thread
    thread = threading.Thread(target=theThread)
    thread.start()
    running = True

def stopThread():
    global running,thread
    running = False
    thread.join()
    GPIO.cleanup()

def theThread():
    while(True):
        time.sleep(1e-3)
        check()
        if(running == False):
            break
def getSpeed():
    global speed

    return speed
    
    
    
if(__name__ == '__main__'):
    startThread()
    
    try:
        while(True):
            time.sleep(1e-3)
            print(getSpeed())
    except KeyboardInterrupt:
        stopThread()
        print("GOODBYTE")

