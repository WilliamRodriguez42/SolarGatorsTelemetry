import serial
import csv
import datetime

voltage = 0
current = 0
csvfile = None
ser = None
csvwriter = None

filename = datetime.datetime.now().strftime("%Y-%m-%d") + ".csv"
print(filename)

def startLogging():
	global csvfile, ser, csvwriter
	try:
		ser = serial.Serial('/dev/ttyACM0', 9600)
	except serial.serialutil.SerialException:
		print("Arduino not connected")

def logData(speed):
	global csvwriter, voltage, current
	if ser is None or (ser.in_waiting > 0):
		voltage = 0
		current = 0

		if ser is not None:
			bs = ser.readline()
			s = bs[:-1].decode('ascii')
			if ',' not in s:
				return

			voltage, current = s.split(',')
			voltage = float(voltage)
			current = float(current)

		time = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-4]

		csvfile = open(filename, 'a+')
		csvwriter = csv.writer(csvfile)
		csvwriter.writerow([time, voltage, current, speed])
		csvfile.close()

def stopLogging():
	global ser

	ser.close()
