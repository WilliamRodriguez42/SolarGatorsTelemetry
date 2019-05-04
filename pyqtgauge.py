##### PyQt Gauge #####
# William Dao
# Example for Solar Gators


import sys
import os

from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtQml import *
from PyQt5.QtWidgets import QApplication

from Car import car_QML
from Car import speed
from Car import Logging
from Car import internet
from Strategy import pro_strats

##### Updater Class #####
# This is how the Qt Quick elements get their data
# Python and Qt interface with signals and slots
# Python creates a signal which it sends to a Qt slot
# We need to import the appropriate module containing the desired variables
# Then we use that module.variable to emit a signal
# e.g. car_QML.variable_name
update_counter = 0
ip_addr = ""
wifi_name = ""
class UpdaterClass(QObject):
	DCL	 = pyqtSignal(str, str, str, str, str, str)
	CCL	 = pyqtSignal(str, str, str, str, str, str)
	relays  = pyqtSignal(int, int, int, int, int, int, int)
	pack	= pyqtSignal(float, float, float, float, float, float, float, float)
	speed   = pyqtSignal(float, float, float)

	def __init__(self, parent=None):
		super(UpdaterClass, self).__init__(parent)

	def dcl_update(self):
		global update_counter, ip_addr, wifi_name
		if update_counter % 150 == 0:
			ip_addr, wifi_name = internet.get_ip_and_name()

		self.DCL.emit(  "Low: "+str(round(car_QML.low_cell_voltage, 2))+" V",
						"High: "+str(round(car_QML.high_cell_voltage, 2))+" V",
						str(round(speed.getSpeed(), 2))+" MPH",
						"Avg: "+str(round(car_QML.average_cell_voltage, 2))+" V",
						ip_addr,
						wifi_name)

		update_counter += 1

	def ccl_update(self):
		self.CCL.emit(	"Summed: "+str(round(car_QML.pack_summed_voltage, 2))+" V",
						"High: "+str(round(car_QML.high_temperature, 2))+" C",
						"SOC: "+str(round(car_QML.adaptive_soc, 2))+" %",
						"Low: "+str(round(car_QML.low_temperature, 2))+" C",
						"Avg: "+str(round(car_QML.average_temperature, 2))+" C",
						"")

	def relays_update(self):
		self.relays.emit(	car_QML.discharge_relay_disabled,
							car_QML.charge_relay_disabled,
							car_QML.charger_safety_disabled,
							car_QML.rs == [1, 1, 1, 1, 1, 1, 1, 1],
							car_QML.always_on_power_status,
							car_QML.is_ready_power_status,
							car_QML.rs == [0, 0, 0, 0, 0, 0, 0, 0])

	def pack_update(self):
		self.pack.emit(	car_QML.average_cell_voltage,
						car_QML.high_temperature,
						car_QML.low_temperature,
						0,
						0, #car_QML.Pack_Instant_Voltage,
						car_QML.adaptive_soc,
						car_QML.average_temperature,
						0) #car_QML.low_cell_voltage)

	def speed_update(self):
		self.speed.emit(speed.getSpeed(), pro_strats.optimumSpeedLow, pro_strats.optimumSpeedHigh)

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
			Logging.logData(speed.getSpeed())
			car_QML.update()		#Call updater function in car_QML
			pro_strats.update()	 #Call updater function in pro_strats
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
	updater.DCL.connect(root.onDCLUpdate)
	updater.CCL.connect(root.onCCLUpdate)
	updater.relays.connect(root.onRelaysUpdate)
	updater.pack.connect(root.onPackUpdate)
	updater.speed.connect(root.onSpeedUpdate)

	# Create timers for update signals
	updateTimer = QTimer()
	updateTimer.start(50)
	updateTimer2 = QTimer()
	updateTimer2.start(50)
	updateTimer3 = QTimer()
	updateTimer3.start(50)
	updateTimer4 = QTimer()
	updateTimer4.start(50)
	updateTimer5 = QTimer()
	updateTimer5.start(50)

	# Connect QTimers to emits
	updateTimer.timeout.connect(updater.dcl_update)
	updateTimer2.timeout.connect(updater.ccl_update)
	updateTimer3.timeout.connect(updater.relays_update)
	updateTimer4.timeout.connect(updater.pack_update)
	updateTimer5.timeout.connect(updater.speed_update)

	# Begin logging
	Logging.startLogging()

	# Run data collection thread
	speed.startThread()
	dataLoop = DataLoop()
	dataLoop.start()

	# Runs the app
	root.showFullScreen()
	app.exec_()

	# Clean up?
	speed.stopThread()
	Logging.stopLogging()

		# Exit the program
	sys.exit()
##### END MAIN #####
