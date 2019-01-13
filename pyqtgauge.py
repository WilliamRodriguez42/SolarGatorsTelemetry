##### PyQt Gauge #####
# William Dao
# Example for Solar Gators


import sys
import os

from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtQml import *
from PyQt5.QtWidgets import QApplication

from Car import speedo


##### Updater Class #####
# This is how the Qt Quick elements get their data
# Python and Qt interface with signals and slots
# Python creates a signal which it sends to a Qt slot
# We need to import the appropriate module containing the desired variables
# Then we use that module.variable to emit a signal
# e.g. car_QML.variable_name
class UpdaterClass(QObject):        
	speed   = pyqtSignal(float)

	def __init__(self, parent=None):
		super(UpdaterClass, self).__init__(parent)

	def speed_update(self):
		self.speed.emit(speedo.speed)
		
##### END Updater Class #####



##### Data Collection Threads #####
# This will pull the data from CANBUS via car_QML.py via car_CAN.py
# This will also calculate strategy data via pro_strats
class DataLoop(QThread):

	def __init__(self):
		QThread.__init__(self)

	def __del__(self):
		self.wait()

	def run(self):
		while True:
			speedo.update()        #Call updater function in placeholder
			self.msleep(100)

##### END Data Collection Threads #####


##### MAIN #####
# We won't really have to touch this part
# besides adding signals and their dependencies
if __name__ == "__main__":
	# Create instance of application
	app = QApplication(sys.argv)

	# Create QML engine
	engine = QQmlApplicationEngine(parent=app)

	# Load QML file
	engine.load('main.qml')

	# Get root object of application
	root = engine.rootObjects()[0]

	# Create instance of signal updater
	updater = UpdaterClass()

	# Create Updater object in QML
	context = engine.rootContext()
	context.setContextProperty("updater", updater)

	# Connect PyQt signals to QML slots
	# onXUpdate corresponds to the Slots made in QML
	updater.speed.connect(root.onSpeedUpdate)

	# Create timers for update signals
	updateTimer = QTimer()
	updateTimer.start(50)

	# Connect QTimers to emits
	updateTimer.timeout.connect(updater.speed_update)

	# Run data collection thread
	dataLoop = DataLoop()
	dataLoop.start()

	# Runs the app
	app.exec_()

        # Exit the program
	sys.exit()
##### END MAIN #####



