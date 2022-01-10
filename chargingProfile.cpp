#include "headers/chargingProfile.h"

ChargingProfile::ChargingProfile() :
    method(ChargingMethod::constantCurrent),
    maxVoltage(0), //mV
    maxCurrent(0), //mA
    maxTemperature(0), //C
    maxTime(0), //mS
    desiredVoltage(0),
    desiredCurrent(0),
    voltageDeltaInterval(0),
    minVoltageDelta(0),
    temperatureDeltaInterval(0),
    maxTemperatureDelta(0),
    endingVoltageDrop(0),
    lookForEndingVoltageDrop(0)
    {}