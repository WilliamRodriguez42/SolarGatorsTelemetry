import random
from Car.car_CAN import *

low_cell_voltage		= report['Low Cell Voltage'].get_true_value()
high_cell_voltage		= report['High Cell Voltage'].get_true_value()
average_cell_voltage	= report['Average Cell Voltage'].get_true_value()
pack_summed_voltage		= report['Pack Summed Voltage'].get_true_value()
low_temperature         = report['Low Temperature'].get_true_value()
high_temperature        = report['High Temperature'].get_true_value()
average_temperature		= report['Average Temperature'].get_true_value()
adaptive_soc			= report['Adaptive SOC'].get_true_value()

rs = bits_from_int(report['Relay State'].value)

discharge_relay_disabled        = 1 - rs[0]
charge_relay_disabled           = 1 - rs[1]
charger_safety_disabled         = 1 - rs[2]
diagnostic_trouble_code_active  = 1 - rs[3]
always_on_power_status          = 1 - rs[4]
is_ready_power_status           = 1 - rs[5]
is_charging_power_status        = 1 - rs[6]

#Temporary data generation function
#since we don't have the CANBus running
def update():
	global low_cell_voltage, 	high_cell_voltage, 	average_cell_voltage, 	pack_summed_voltage, 	low_temperature, 	high_temperature, 	average_temperature, 	adaptive_soc, 	rs, 	discharge_relay_disabled, 	charge_relay_disabled, 	charger_safety_disabled, 	diagnostic_trouble_code_active, 	always_on_power_status, 	is_ready_power_status, 	is_charging_power_status

	low_cell_voltage		= report['Low Cell Voltage'].get_true_value()
	high_cell_voltage		= report['High Cell Voltage'].get_true_value()
	average_cell_voltage	= report['Average Cell Voltage'].get_true_value()
	pack_summed_voltage		= report['Pack Summed Voltage'].get_true_value()
	low_temperature         = report['Low Temperature'].get_true_value()
	high_temperature        = report['High Temperature'].get_true_value()
	average_temperature		= report['Average Temperature'].get_true_value()
	adaptive_soc			= report['Adaptive SOC'].get_true_value()

	rs = bits_from_int(report['Relay State'].value)

	discharge_relay_disabled        = 1 - rs[0]
	charge_relay_disabled           = 1 - rs[1]
	charger_safety_disabled         = 1 - rs[2]
	diagnostic_trouble_code_active  = 1 - rs[3]
	always_on_power_status          = 1 - rs[4]
	is_ready_power_status           = 1 - rs[5]
	is_charging_power_status        = 1 - rs[6]

def close():
	can.closeEvent()

can = CAN()

#Don't need a main if the PyQt gauge is running :-)
##if __name__ == '__main__':
##	print('Pack Current ', Pack_Current)
##	update_variables()
##	print('Pack Current ', Pack_Current)
##
##	close()
