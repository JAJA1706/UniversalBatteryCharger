#pragma once
#include "chargingProfile.h"
#include "iterator.h"

class ChargingMonitor
{
private:
    static constexpr int TIMED_TABLE_SIZE = 10;
    static constexpr int MEAN_TABLE_SIZE = 20;
    const int REQUIRED_RESULTS_TO_END;

    unsigned long batteryStartTime;
    unsigned long batteryChargingTime;
    unsigned long profileStartTime;
    unsigned long profileChargingTime;
    int endingStateCounter;
    int wrongStateCounter;
    unsigned int loopCount;

    unsigned long voltageTimer;
    Iterator voltageTableIter;
    bool newVoltageData;
    double voltageInIntervals[TIMED_TABLE_SIZE];
    unsigned long temperatureTimer;
    Iterator temperatureTableIter;
    bool newTemperatureData;
    double temperatureInIntervals[TIMED_TABLE_SIZE];


    Iterator meanTableIter;
    double meanVoltageTable[MEAN_TABLE_SIZE];
    double meanBatteryVoltage;
    double maxRecordedVoltage;
    
    double percentComplete; //do zrobienia
    bool finalPhase; //do zrobienia

    int checkForTerminalValues(const ChargingProfile& profile ) const;
    void checkTimer();
    void fillTimedTables(const unsigned long voltageInvervalTime, const unsigned long temperatureInvervalTime);
    void calculateMeanVoltage();
    void savePotentialMaxVoltage();
    void clearBufferTables();
    unsigned long getPassedTime(const unsigned startTime) const;
    bool maxProfileValueHasBeenExceeded(const ChargingProfile& profile) const;
public:
    ChargingMonitor();
    void resetProfileTimer();
    void batteryChargingStarted();
    void batteryChargingEnded();
    int checkForEndOfTheCharge(const ChargingProfile& profile);
};