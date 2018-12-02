#include "tmmessage.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>


TMMessage::TMMessage()
{
    WattHours = 0.0;              // kWh
    PackCurrent = 0.0;            // Amps
    PackInstantVoltage = 0.0;     // Volts
    StateOfCharge = 0.0;          // %
    RelayStatus = 0;
    PackAmpHours = 0.0;           // Ah
    CurrentLimitStatus = 0;
    HighTemperature = 0;        // C
    LowTemperature = 0;         // C

}

bool TMMessage::getNextMsg(std::ifstream &fp) {
    std::string str;
    // get the next line from the file
    std::getline(fp,str);

    if (!fp.good()) {
    	return false;
    }

    auto pos = str.find(",");   // read past the comma in the string
    std::string token = str.substr(0,pos);
    std::string rem = str.substr(pos+1);
    std::stringstream srem(rem);

    if (token == "Watt Hours"){
        srem >> WattHours;              // kWh
    }
    if (token == "Pack Current"){
        srem >> PackCurrent;            // Amps
    }
    if (token == "Pack Instant Voltage"){
        srem >> PackInstantVoltage;     // Volts
    }
    if (token == "State Of Charge"){
        srem >> StateOfCharge;          // %
    }
    if (token == "Relay Status"){
        srem >> RelayStatus;
    }
    if (token == "Pack Amp Hours"){
        srem >> PackAmpHours;           // Ah
    }
    if (token == "Current Limit Status"){
        srem >> CurrentLimitStatus;
    }
    if (token == "High Temperature"){
        srem >> HighTemperature;        // C
    }
    if (token == "Low Temperature"){
        srem >> LowTemperature;         // C
    }

    return true;
}
