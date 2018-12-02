#ifndef TMMESSAGE_H
#define TMMESSAGE_H
#include <fstream>

class TMMessage
{
public:
    TMMessage();
    bool getNextMsg(std::ifstream&);

    double  WattHours;              // kWh
    double  PackCurrent;            // Amps
    double  PackInstantVoltage;     // Volts
    double  StateOfCharge;          // %
    int     RelayStatus;
    double  PackAmpHours;           // Ah
    int     CurrentLimitStatus;
    int     HighTemperature;        // C
    int     LowTemperature;         // C
};

#endif // TMMESSAGE_H
