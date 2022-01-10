#pragma once

enum class ChargingMethod
{
    constantCurrent,
    constantVoltage,
    trickleCharge
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
};
