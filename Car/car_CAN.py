import sys
import serial
import threading
import os
import time
import math
import numpy as np
import datetime
import glob

# The code to run this thing on boot is the last line in the file:
# sudo nano ~/.config/lxsession/LXDE-pi/autostart

class SubMessage:
	def __init__(self, num, text, print_if_low=False):
		self.text = text
		self.num = num
		self.print_if_low = print_if_low

report = {}
class Message:
	def __init__(self, text, multiplier, units, num_nibbles, signed=True):
		self.text = text
		self.multiplier = multiplier
		self.units = units
		self.num_nibbles = num_nibbles
		self.signed = signed
		self.sub_messages = None
		self.value = 0xFFFF

		global report
		report[self.text] = self

	def get_true_value(self):
		return bytes_to_int(self.value, self.signed) * self.multiplier

blank = Message('', 1, '', 2)

# All the submessages in current limit status
current_limit_status = (
SubMessage(0, 'DCL Low SOC'),
SubMessage(1, 'DCL High Cell Resistance'),
SubMessage(2, 'DCL Temperature'),
SubMessage(3, 'DCL Low Cell Voltage'),
SubMessage(4, 'DCL Low Pack Voltage'),
SubMessage(6, 'DCL Voltage Failsafe'),
SubMessage(9, 'CCL High SOC'),
SubMessage(10, 'CCL High Cell Resistance'),
SubMessage(11, 'CCL Temperature'),
SubMessage(12, 'CCL High Cell Voltage'),
SubMessage(13, 'CCL High Pack Voltage'),
SubMessage(14, 'CCL Charger Latch'),
)

# All of the submessages for the relay status
relay_status = [
SubMessage(0, 'discharge relay disabled', True),
SubMessage(1, 'charge relay disabled', True),
SubMessage(2, 'charger safety disabled', True),
SubMessage(3, 'diagnostic trouble code active', True),
SubMessage(5, 'always on power status', True),
SubMessage(6, 'is ready power status', True),
SubMessage(7, 'is charging power status', True),
]

# Here we will name all of the variables coming in through CAN and associate them to their ID
# It works in the form of:
# Name (unit_multiplier units)
# Leave blank if it's coming in through CAN but you don't want it to print
ids = {
	b'6B1' : [
		Message('Pack DCL', 1, '', 4),
		Message('Pack CCL', 1, '', 4),
		Message('High Temperature', 1, 'C', 2),
		Message('Low Temperature', 1, 'C', 2)
	],
	b'6B0' : [
		Message('Pack Current', 0.1, 'A', 4),
		Message('Pack Instant Voltage', 0.1, 'V', 4),
		Message('Pack SOC', 0.5, '%', 2, False),
		Message('Relay State', 1, '', 4, False)
	],
	b'720' : [

	],
	b'721' : [

	],
	b'640' : [

	]
}

#report['Relay Status'].sub_messages = relay_status
#report['Current Limit Status'].sub_messages = current_limit_status

# Convert bytes to integer
def bytes_to_int(bts, signed = True):
	if signed:
		return np.int16(bts)
	else:
		return bts

# Convert integer to bits
def bits_from_int(i):
	j = i
	result = []
	while j > 0:
		k = j % 2
		result.append(k)
		j //= 2

	return result

class CAN:
	def __init__(self):
		self.closing = False # Set to true if we are trying to close the program

		# Initialize serial communication to CANdapter
		self.initSerial()

	def print_ids(self):
		for id, messages in ids.items():
			print(id)
			for message in messages:
				print('\t', message.text)
				print('\t\t', message.get_true_value())

	def initSerialThread(self):
		# The name of the USB port that the CANdapter is hooked up to (it's always the same on the Raspi)
		prefix = '/dev/ttyUSB'

		# Find the CANdapter
		while not self.closing:
			for i in range(10):
				if os.path.exists(prefix + str(i)):
					self.ser = serial.Serial(prefix + str(i))
					break

			if not self.ser:
				print("Could not find CANdapter, trying again")
				time.sleep(2)
			else:
				print("Successfully connected to CANdapter")
				break

		if self.closing:
			return

		# Check the manual for more information on this stuff
		# It's supposed to set the baudrate and start reading
		self.ser.write(b'S')
		self.ser.write(b'6')
		self.ser.write(b'\r')

		self.ser.write(b'o')
		self.ser.write(b'\r')

		message = b''
		while not self.closing:
			self.print_ids();
			r = self.ser.read() # Read

			if r == b't': # For some reason they like to end with a t so look for that
				id = message[:3] # Get the first three nibbles for our ID
				if id not in ids: # If we don't recognize this ID snitch on it (You'll probably get one of these when you first start the Pi)
					#print('Error: ' + str(id))
					message = b''
					continue

				#report['id'] = id # I don't remember what this was for

				# Parse out all of the messages in this ID
				start = 4
				for m in ids[id]:
					b = m.num_nibbles
					if m != blank:
						rep = message[start:start+b]
						print(id, m, rep)
						if (m.num_nibbles == 4):
							m.value = (int(rep[2:4], 16) << 8) | int(rep[0:2], 16)
						else:
							m.value = int(rep, 16)

					start += b

				message = b''
			else:
				message += r

	def initSerial(self):
		# ser is our serial object, but right now we will set it to False
		self.ser = False

		# Initialize the serial in a different thread
		initThread = threading.Thread(target=self.initSerialThread)
		initThread.daemon = True
		initThread.start()

	def closeEvent(self):
		# Close my ass cheeks
		if self.ser:
			self.ser.close()
		self.closing = True
