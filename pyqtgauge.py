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


##### Updater Class #####
# This is how the Qt Quick elements get their data
# Python and Qt interface with signals and slots
# Python creates a signal which it sends to a Qt slot
# We need to import the appropriate module containing the desired variables
# Then we use that module.variable to emit a signal
# e.g. car_QML.variable_name
class UpdaterClass(QObject):        
	DCL     = pyqtSignal(float, float, float, float, float, float)
	CCL     = pyqtSignal(float, float, float, float, float, float)
	relays  = pyqtSignal(int, int, int, int, int, int, int)
	pack    = pyqtSignal(float, float, float, float, float, float, float, float)

	def __init__(self, parent=None):
		super(UpdaterClass, self).__init__(parent)

	def dcl_update(self):
		self.DCL.emit(car_QML.DCL_Low_SOC, car_QML.DCL_High_Cell_Resistance, car_QML.DCL_Temperature, car_QML.DCL_Low_Cell_Voltage, car_QML.DCL_Low_Pack_Voltage, car_QML.DCL_Voltage_Failsafe)

	def ccl_update(self):
		self.CCL.emit(car_QML.CCL_High_SOC, car_QML.CCL_High_Cell_Resistance, car_QML.CCL_Temperature, car_QML.CCL_High_Cell_Voltage, car_QML.CCL_High_Pack_Voltage, car_QML.CCL_Charger_Latch)

	def relays_update(self):
		self.relays.emit(car_QML.discharge_relay_disabled, car_QML.charge_relay_disabled, car_QML.charger_safety_disabled, car_QML.diagnostic_trouble_code_active, car_QML.always_on_power_status, car_QML.is_ready_power_status, car_QML.is_charging_power_status)

	def pack_update(self):
		self.pack.emit(car_QML.Pack_Amp_Hours, car_QML.High_Temperature, car_QML.Low_Temperature, car_QML.Pack_Current, car_QML.Pack_Instant_Voltage, car_QML.State_Of_Charge, car_QML.Relay_Status, car_QML.Watt_Hours)
##### END Updater Class #####



##### Data Collection Threads #####
# This will pull the data from CANBUS via car_QML.py via car_CAN.py
class DataLoop(QThread):

	def __init__(self):
		QThread.__init__(self)

	def __del__(self):
		self.wait()

	def run(self):
		while True:
			car_QML.update()        #Call updater function in car_QML
			self.msleep(150)

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

	# Create timers for update signals
	updateTimer = QTimer()
	updateTimer.start(50)
	updateTimer2 = QTimer()
	updateTimer2.start(50)
	updateTimer3 = QTimer()
	updateTimer3.start(50)
	updateTimer4 = QTimer()
	updateTimer4.start(50)

	# Connect QTimers to emits
	updateTimer.timeout.connect(updater.dcl_update)
	updateTimer2.timeout.connect(updater.ccl_update)
	updateTimer3.timeout.connect(updater.relays_update)
	updateTimer4.timeout.connect(updater.pack_update)

	# Run data collection thread
	dataLoop = DataLoop()
	dataLoop.start()

	# Runs the app, and exits Python on GUI exit
	sys.exit(app.exec_())
##### END MAIN #####



