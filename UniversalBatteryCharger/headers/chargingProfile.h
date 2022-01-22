#pragma once

enum class ChargingMethod
{
    constantCurrent,
    constantVoltage,
};

struct ChargingProfile
{
    ChargingMethod method;
    double maxVoltage;
    double maxCurrent;
    double maxTemperature;
    unsigned long maxTime;

    double desiredVoltage;
    double desiredCurrent;
    
    unsigned long voltageDeltaInterval;
    double minVoltageDelta;

    unsigned long temperatureDeltaInterval;
    double maxTemperatureDelta;

    double endingVoltageDrop;
    bool lookForEndingVoltageDrop;

    ChargingProfile();
    ChargingProfile( ChargingMethod _method, double _maxVoltage, double _maxCurrent,
    double _maxTemperature, unsigned long _maxTime, double _desiredVoltage, double _desiredCurrent,
    unsigned long _voltageDeltaInterval, double _minVoltageDelta, unsigned long _temperatureDeltaInterval,
    double _maxTemperatureDelta, double _endingVoltageDrop, bool _lookForEndingVoltageDrop);
    
    void print();
};
