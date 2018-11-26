//Attempting to re-implement the CAN stuff from car_CAN.py
#include "car_can.h"

car_CAN::car_CAN()
{
	this->print_if_low = false;
}
//import sys

//import serial

//import threading

//import os

//import time

//import math

//import numpy as np

//import datetime

//import glob



// Other bullshit

#define running_on_pi false; // Change to true if you are running this program on a raspberry pi



// Logging

int max_files_to_keep = 10; // Maximum log files to keep



// Don't use GPIO pins if we are not on a Raspi

if (running_on_pi) {

	import RPi.GPIO as GPIO;

}

// The code to run this thing on boot is the last line in the file:

// sudo nano ~/.config/lxsession/LXDE-pi/autostart




SubMessage::SubMessage(int num, QString text, bool print_if_low) {
	this->text = text;
	this->num = num;
	this->print_if_low = print_if_low;
}
	/*def __init__(self, num, text, print_if_low){

		this.text = text;

		this.num = num;

		this.print_if_low = print_if_low;
	}*/

report = {};

		


	/*def __init__(self, text, multiplier, units, num_nibbles, isSigned=False){

		self.text = text;

		self.multiplier = multiplier;

		self.units = units;

		self.num_nibbles = num_nibbles;

		self.signed = signed;

		self.sub_messages = None;

		self.value = 0xFFFF;



		global report;

		report[self.text] = self;
	}*/
Message::Message(QString text, int multiplier, enum units the_units, int num_nibbles, bool isSigned) {
	this->text = text;

	this->multiplier = multiplier;
	
	this->the_units = the_units;
	
	this->num_nibbles = num_nibbles;
	
	this->isSigned = isSigned; //I've changed the name of signed to isSigned because it was doing something weird
	
	this->sub_messages = nullptr;//convert this to a QList within messages I guess?
	//sub_messages is a data structure that holds SubMessage objects I think
	this->value = 0xFFFF;
}

int Message::get_true_value(){
	return bytes_to_int(this->value, this->isSigned) * this->multiplier;
}


blank = Message('', 1, '', 1);



// All the submessages in current limit status

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

);



// All of the submessages for the relay status

relay_status = [

SubMessage(0, 'discharge relay disabled', True),

SubMessage(1, 'charge relay disabled', True),

SubMessage(2, 'charger safety disabled', True),

SubMessage(3, 'diagnostic trouble code active', True),

SubMessage(5, 'always on power status', True),

SubMessage(6, 'is ready power status', True),

SubMessage(7, 'is charging power status', True),

];



// Here we will name all of the variables coming in through CAN and associate them to their ID

// It works in the form of:

// Name (unit_multiplier units)

// Leave blank if it's coming in through CAN but you don't want it to print

ids = {

	b'6B1' : [

		blank,

		Message('Pack Amp Hours', 0.1, 'Ah', 4),

		Message('Current Limit Status', 1, '', 4, True),

		Message('High Temperature', 1, 'C', 2),

		Message('Low Temperature', 1, 'C', 2),

		blank,

		blank,

		blank,

		blank

	],

	b'6AE' : [

		blank,

		Message('Pack Current', 0.1, 'A', 4, True),

		Message('Pack Instant Voltage', 0.1, 'V', 4),

		Message('State Of Charge', 0.5, '%', 2),

		blank,

		blank,

		Message('Relay Status', 1, '', 2),

		blank

	],

	b'000' : [

		blank,

		Message('Watt Hours', 0.00001, 'kWh', 16)

	]

};



report['Relay Status'].sub_messages = relay_status;

report['Current Limit Status'].sub_messages = current_limit_status;



// Convert bytes to integer

int bytes_to_int(char[] bts, signed = True){ //this assumes little-endian (need to check this) 
	int a = int((unsigned char)(buffer[0]) << 8 |
			(unsigned char)(buffer[1]) << 0); //this will work for unsigned values
	return a;
	/*if signed{
		//return np.int16(bts);
		
	}
	
	else {
		return a;
	}*/
}


// Convert integer to bits

def bits_from_int(i) {

	j = i;

	result = [];

	while j > 0 {

		k = j % 2;

		result.append(k);

		j /= 2;
		}


	return result;
}

CAN::CAN() {
	this->closing = False; // Set to true if we are trying to close the program
	// Delete last log file if there are more than 9
	files = glob.glob('Logs/*.txt');
	while (len(files) >= 10) {	
		os.remove(sorted(files)[0]);
			files = glob.glob('*.txt');
		}
		// Create our log file
		this->log_file = open('Logs/' + str(datetime.datetime.now()) + '.txt', 'w');
	
		// More shit that is exclusive to the pi
	
		if (running_on_pi) {
			GPIO.setmode(GPIO.BCM);
			GPIO.setup(15, GPIO.IN);
		}
			// Initialize serial communication to CANdapter
			this->initSerial();
			// Start a loop for getting speed
			"""s = threading.Thread(target=self.getSpeed)
			s.daemon = True;
			s.start();"""
			// Save default information to our log (mainly for debugging)
			this->log();
}
		

void CAN::initSerialThread() {
	// The name of the USB port that the CANdapter is hooked up to (it's always the same on the Raspi)
		prefix = '/dev/ttyUSB';
		// Find the CANdapter
		while (!this->closing) {
			for (i in range(10)) {
				if (os.path.exists(prefix + str(i))) {
					this->ser = serial.Serial(prefix + str(i));
					break;
				}
			}
			if (not this->ser) {
				print("Could not find CANdapter, trying again");
				time.sleep(2);
			}
			else {
				print("Successfully connected to CANdapter");
				break;
			}
		}

		if (this->closing){
			return
		}


		// Check the manual for more information on this stuff

		// It's supposed to set the baudrate and start reading

		this->ser.write(b'S');

		this->ser.write(b'6');

		this->ser.write(b'\r');



		this->ser.write(b'o');

		this->ser.write(b'\r');



		message = b'';

		while (not this->closing) {
			r = this->ser.read(); // Read
			if r == b't'{ // For some reason they like to end with a t so look for that
				id = message[:3]; // Get the first three nibbles for our ID
				if (id not in ids) { // If we don't recognize this ID snitch on it (You'll probably get one of these when you first start the Pi)
					print('Error: ' + str(id));
					message = b'';
					continue;
				}

				//report['id'] = id # I don't remember what this was for
				// Parse out all of the messages in this ID
				start = 3;
				for m in ids[id]:
					if (m != blank) {
						b = m.num_nibbles;
						rep = message[start:start+b];
						m.value = int(rep); // Convert to int 32
					}
					start += b;
				// Log shit
				this->log();
				message = b'';
			}
			else {

				message += r;
			}
		}
	}


void CAN::initSerial() {

		// ser is our serial object, but right now we will set it to False

		this->ser = False;



		// Initialize the serial in a different thread

		initThread = threading.Thread(target=this->initSerialThread);

		initThread.daemon = True;

		initThread.start();
	}


CAN::log() { // Logs all data from report into our log file
	for (id, messages in sorted(ids.items())) {
		for (message in messages){
			if (message != blank) {
				this->log_file.write(str(message.text) + ', ' + str(message.get_true_value()) + ' ' + message.units + '\n');
			}
		}
	}
	this->log_file.flush();
}


CAN::getSpeed() { 		// Get the goddamn speed for the love of God
		while (not this->closing){

			if (running_on_pi) {

				curr = GPIO.input(15); // Recieve on gpio 15

				currtime = time.time(); // The current time, bitch



				delta = currtime - this->lasttime; // The change in time, it's fucking CRAZY

				mph = 1/delta * this->wheel_circumference * this->inches_to_miles * this->seconds_to_hours / 6; // Convert to MPH (divide by 6 because we have six magnets that we are reading from)



				if (all([p == 1 for p in this->prev[1:]]) and this->prev[0] == 0) { // Check if there is one 0 followed by nine 1's

					if (abs((this->prevMPH[-1] - mph) / delta) < this->valid_accel){ // Make sure we didn't just accelerate like crazy because that's probably wrong

						// Record our speed

						this->prevMPH.append(mph);

						if (len(this.prevMPH) > this->record_length){

							this->prevMPH = this->prevMPH[1:];
							}


						avgMPH = 0;

						for (s in this->prevMPH) {

							avgMPH += s;
							}


						avgMPH /= this->record_length;



						this->speed = avgMPH;



						this->lasttime = currtime;
					}
				}

				this->prev.append(curr);

				if (len(this->prev) > this->valid_length) {

					this->prev = this->prev[1:];
				}
			}
		}
}


CAN::closeEvent() { // Close my ass cheeks
	this->log_file.close();
	if (this->ser) {
		this->ser.close();
	}
	this->closing = True;
}

