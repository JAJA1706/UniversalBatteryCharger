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

ChargingProfile::ChargingProfile( ChargingMethod _method, double _maxVoltage, double _maxCurrent,
    double _maxTemperature, unsigned long _maxTime, double _desiredVoltage, double _desiredCurrent,
    unsigned long _voltageDeltaInterval, double _minVoltageDelta, unsigned long _temperatureDeltaInterval,
    double _maxTemperatureDelta, double _endingVoltageDrop, bool _lookForEndingVoltageDrop) 
    : 
    method(_method),
    maxVoltage(_maxVoltage),
    maxCurrent(_maxCurrent),
    maxTemperature(_maxTemperature),
    maxTime(_maxTime),
    desiredVoltage(_desiredVoltage),
    desiredCurrent(_desiredCurrent),
    voltageDeltaInterval(_voltageDeltaInterval),
    minVoltageDelta(_minVoltageDelta),
    temperatureDeltaInterval(_temperatureDeltaInterval),
    maxTemperatureDelta(_maxTemperatureDelta),
    endingVoltageDrop(_endingVoltageDrop),
    lookForEndingVoltageDrop(_lookForEndingVoltageDrop) {}