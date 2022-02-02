#pragma once
#include "chargingProfile.h"
#include "iterator.h"
#include "sensors.h"

class ChargingMonitor
{
private:
    static constexpr int TIMED_TABLE_SIZE = 10;
    static constexpr int MEAN_TABLE_SIZE = 20;
    const int REQUIRED_RESULTS_TO_END;
    const int REQUIRED_INTERVAL_RESULTS_TO_END;

    Sensors* sensors;

    unsigned long batteryStartTime;
    unsigned long batteryChargingTime;
    unsigned long profileStartTime;
    unsigned long profileChargingTime;
    int endingStateCounter;
    int wrongStateCounter;
    unsigned int loopCount;

    unsigned long voltageTimer;
    InfiniteIterator voltageTableIter;
    bool newVoltageData;
    double voltageInIntervals[TIMED_TABLE_SIZE];
    unsigned long temperatureTimer;
    InfiniteIterator temperatureTableIter;
    bool newTemperatureData;
    double temperatureInIntervals[TIMED_TABLE_SIZE];


    InfiniteIterator meanTableIter;
    double meanVoltageTable[MEAN_TABLE_SIZE];
    double meanBatteryVoltage;
    double maxRecordedVoltage;
    
    double storedEnergy;
    double energyTimer;

    int checkForTerminalValues(const ChargingProfile& profile ) const;
    void checkTimer();
    void fillTimedTables(const unsigned long voltageInvervalTime, const unsigned long temperatureInvervalTime);
    void calculateMeanVoltage();
    void savePotentialMaxVoltage();
    void clearBufferTables();
    void resetStateCounters();
    unsigned long getPassedTime(const unsigned long startTime) const;
    bool maxProfileValueHasBeenExceeded(const ChargingProfile& profile) const;
    void calculateStoredEnergy();
public:
    ChargingMonitor();
    void setSensors(Sensors* sensorsPtr);
    void batteryChargingStarted();
    void batteryChargingEnded();
    void profileChargingEnded();
    int checkForEndOfTheCharge(const ChargingProfile& profile);
    double getCompletePercentage(const double batteryCapacity);
};