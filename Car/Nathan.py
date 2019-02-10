import serial
import csv
import datetime

filename = datetime.datetime.now().strftime("%Y-%m-%d") + ".csv"
ser = serial.Serial('/dev/ttyACM0', 9600)
csvfile = open(filename, 'a+')
csvwriter = csv.writer(csvfile)

try:
	while True:
		if (ser.in_waiting > 0):
			bs = ser.readline()
			s = bs[:-1].decode('ascii')
			voltage, current = s.split(',')
			time = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-4]
			csvwriter.writerow([voltage, current])
except KeyboardInterrupt:
	print("Writing file")
	
csvfile.close()
