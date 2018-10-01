##### PyQt Gauge #####
# William Dao
# Example for Solar Gators


import sys
import os

from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtQml import *
from PyQt5.QtWidgets import QApplication


##### GLOBALS #####
voltage = 0
current = 0
load    = 0
temp    = 0
speed   = 0
runtime = 0
##### END GLOBALS #####


##### Updater Class #####
# This is how the Qt Quick elements get their data
# Python and Qt interface with signals and slots
# Python creates a signal which it sends to a Qt slot
class UpdaterClass(QObject):
	normal = pyqtSignal(float, float, float, float, float, int)

	def __init__(self, parent=None):
		super(UpdaterClass, self).__init__(parent)


	def normal_update(self):
		self.normal.emit(voltage, current, load, temp, speed, runtime)
##### END Updater Class #####



##### Data Generating Threads #####
# This just gives us some data to work with
class NormalLoop(QThread):

	def __init__(self):
		QThread.__init__(self)

	def __del__(self):
		self.wait()

	def run(self):
		# Refer to the globals as global so we can
		# change them by reference
		global voltage
		global current
		global load
		global temp
		global speed
		global runtime

		while True:
			if voltage >= 20 :
				voltage = 0
			else :
				voltage += .000005

			if current >= 50 :
				current = 0
			else :
				current += .000005
				
			if load >= 100 :
				load = 0
			else :
				load += .000005

			if temp >= 250 :
				temp = 0 
			else :
				temp += .000005

			if speed >= 100 :
				speed = 0
			else :
				speed += .000005

			if runtime >= 900 :
				runtime = 0
			else :
				runtime += .0000015
			
			self.sleep(0)
##### END Data Generating Threads #####



##### MAIN #####
# We won't really have to touch this part
# besides adding signals and their dependencies
if __name__ == "__main__":
	# Create instance of application
	app = QApplication(sys.argv)

	# Create QML engine
	engine = QQmlApplicationEngine()

	# Load QML file
	engine.load('main.qml')

	# Get root object of application
	root = engine.rootObjects()[0]

	# Create instance of signal updater
	updater = UpdaterClass()

	# Create Updater object in QML
	context = engine.rootContext()
	context.setContextProperty("updater", updater)

	# Connect normal update signal to QML slot
	# onNormalUpdate corresponds to the Slot made in QML
	updater.normal.connect(root.onNormalUpdate)     

	# Create timer for normal update signal
	normalTimer = QTimer()
	normalTimer.start(50)

	# Connect normal update QTimer normal update data generation thread
	normalTimer.timeout.connect(updater.normal_update)

	# Run normal update data generation thread
	normalLoop = NormalLoop()
	normalLoop.start()

	# Runs the app, and exits Python on GUI exit
	sys.exit(app.exec_())
##### END MAIN #####



