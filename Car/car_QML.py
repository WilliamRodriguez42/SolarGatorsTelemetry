import random
from Car.car_CAN import *

DCL_Low_SOC                 = 0
DCL_High_Cell_Resistance    = 0
DCL_Temperature             = 0
DCL_Low_Cell_Voltage        = 0
DCL_Low_Pack_Voltage        = 0
DCL_Voltage_Failsafe        = 0

CCL_High_SOC                = 0
CCL_High_Cell_Resistance    = 0
CCL_Temperature             = 0
CCL_High_Cell_Voltage       = 0
CCL_High_Pack_Voltage       = 0
CCL_Charger_Latch           = 0

discharge_relay_disabled        = 0
charge_relay_disabled           = 0
charger_safety_disabled         = 0
diagnostic_trouble_code_active  = 0
always_on_power_status          = 0
is_ready_power_status           = 0
is_charging_power_status        = 0

Pack_Amp_Hours          = 0
High_Temperature        = 0
Low_Temperature         = 0
Pack_Current            = 0
Pack_Instant_Voltage    = 0
State_Of_Charge         = 0
Relay_State            = 0
Watt_Hours              = 0

rs = 0

def update_temp():
	global DCL_Low_SOC, DCL_High_Cell_Resistance, DCL_Temperature, DCL_Low_Cell_Voltage, DCL_Low_Pack_Voltage, DCL_Voltage_Failsafe, DCL_High_SOC, CCL_High_Cell_Resistance, CCL_Temperature, CCL_High_Cell_Voltage, CCL_High_Pack_Voltage, CCL_Charger_Latch, discharge_relay_disabled, charge_relay_disabled, charger_safety_disabled, diagnostic_trouble_code_active, always_on_power_status, is_ready_power_status, is_charging_power_status, Pack_Amp_Hours, High_Temperature, Low_Temperature, Pack_Current, Pack_Instant_Voltage, State_Of_Charge, Relay_State, Watt_Hours

	cls = bits_from_int(report['Current Limit Status'].value)

	buff = [0] * (len(current_limit_status) - len(cls))
	buff.extend(cls)
	cls = buff

	rs = bits_from_int(report['Relay State'].value)

	buff = [0] * (len(Relay_State) - len(rs))
	buff.extend(rs)
	rs = buff

	DCL_Low_SOC                 = cls[0]
	DCL_High_Cell_Resistance    = cls[1]
	DCL_Temperature             = cls[2]
	DCL_Low_Cell_Voltage        = cls[3]
	DCL_Low_Pack_Voltage        = cls[4]
	DCL_Voltage_Failsafe        = cls[5]
	CCL_High_SOC                = cls[6]
	CCL_High_Cell_Resistance    = cls[7]
	CCL_Temperature             = cls[8]
	CCL_High_Cell_Voltage       = cls[9]
	CCL_High_Pack_Voltage       = cls[10]
	CCL_Charger_Latch           = cls[11]

	discharge_relay_disabled        = 1 - rs[0]
	charge_relay_disabled           = 1 - rs[1]
	charger_safety_disabled         = 1 - rs[2]
	diagnostic_trouble_code_active  = 1 - rs[3]
	always_on_power_status          = 1 - rs[4]
	is_ready_power_status           = 1 - rs[5]
	is_charging_power_status        = 1 - rs[6]

	Pack_Amp_Hours          = report['Pack Amp Hours'].get_true_value()
	High_Temperature        = report['High Temperature'].get_true_value()
	Low_Temperature         = report['Low Temperature'].get_true_value()
	Pack_Current            = report['Pack Current'].get_true_value()
	Pack_Instant_Voltage    = report['Pack Instant Voltage'].get_true_value()
	State_Of_Charge         = report['Pack SOC'].get_true_value()
	Relay_State            = report['Relay State'].get_true_value()
	Watt_Hours              = Pack_Instant_Voltage * Pack_Current * 0.0001;


#Temporary data generation function
#since we don't have the CANBus running
def update():
	global DCL_Low_SOC, DCL_High_Cell_Resistance, DCL_Temperature, DCL_Low_Cell_Voltage, DCL_Low_Pack_Voltage, DCL_Voltage_Failsafe, CCL_High_SOC, CCL_High_Cell_Resistance, CCL_Temperature, CCL_High_Cell_Voltage, CCL_High_Pack_Voltage, CCL_Charger_Latch, discharge_relay_disabled, charge_relay_disabled, charger_safety_disabled, diagnostic_trouble_code_active, always_on_power_status, is_ready_power_status, is_charging_power_status, Pack_Amp_Hours, High_Temperature, Low_Temperature, Pack_Current, Pack_Instant_Voltage, State_Of_Charge, Relay_State, Watt_Hours, rs

	DCL_Low_SOC                 = 1
	DCL_High_Cell_Resistance    = 1
	DCL_Temperature             = 1
	DCL_Low_Cell_Voltage        = 1
	DCL_Low_Pack_Voltage        = 1
	DCL_Voltage_Failsafe        = 1

	CCL_High_SOC                = 1
	CCL_High_Cell_Resistance    = 1
	CCL_Temperature             = 1
	CCL_High_Cell_Voltage       = 1
	CCL_High_Pack_Voltage       = 1
	CCL_Charger_Latch           = 1

	Pack_Amp_Hours          = 1 #report['Pack Amp Hours'].get_true_value()
	High_Temperature        = report['High Temperature'].get_true_value()
	Low_Temperature         = report['Low Temperature'].get_true_value()
	Pack_Current            = report['Pack Current'].get_true_value()
	Pack_Instant_Voltage    = report['Pack Instant Voltage'].get_true_value()
	State_Of_Charge         = report['Pack SOC'].get_true_value()
	Relay_State            = report['Relay State'].get_true_value()
	Watt_Hours              = Pack_Instant_Voltage * Pack_Current * 0.0001;

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
