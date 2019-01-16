import RPi.GPIO as GPIO
import time
import threading
import math

GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.IN)
current = 0
previous = 0
beginning = time.time()
running = True
thread = None
speed = 0
hist_length = 6
history = [beginning] * hist_length

def histor():
	global history, speed

	history.append(time.time())
	del history[0]

	dtime = history[-1] - history[0]
	speed = hist_length/dtime

def check():
	global current,previous,tps,speed
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

	_speed = speed/6
	_speed = _speed*60*60
	diameter = 24
	circumference = diameter * math.pi
	distance = circumference/63360
	_speed = distance * _speed

	return _speed

if(__name__ == '__main__'):
	startThread()

	try:
		while(True):
			time.sleep(1e-3)
			print(getSpeed())
	except KeyboardInterrupt:
		stopThread()
		print("GOODBYTE")
