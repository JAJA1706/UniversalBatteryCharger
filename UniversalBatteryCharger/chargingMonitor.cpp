#include "headers/chargingMonitor.h"
#include "headers/sensors.h"
#include "Arduino.h"

constexpr int ChargingMonitor::TIMED_TABLE_SIZE;
constexpr int ChargingMonitor::MEAN_TABLE_SIZE;

ChargingMonitor::ChargingMonitor() :
    REQUIRED_RESULTS_TO_END(3),
    REQUIRED_INTERVAL_RESULTS_TO_END(2),
    batteryStartTime(0),
    batteryChargingTime(0),
    profileStartTime(0),
    profileChargingTime(0),
    endingStateCounter(0),
    wrongStateCounter(0),
    loopCount(0),
    voltageTimer(0),
    voltageTableIter(TIMED_TABLE_SIZE),
    newVoltageData(false),
    temperatureTimer(0),
    temperatureTableIter(TIMED_TABLE_SIZE),
    newTemperatureData(false),
    meanTableIter(MEAN_TABLE_SIZE),
    meanBatteryVoltage(0),
    maxRecordedVoltage(0),
    percentComplete(0),
    finalPhase(true)
{
    clearBufferTables();
}

int ChargingMonitor::checkForEndOfTheCharge(const ChargingProfile& profile)
{
    checkTimer();
    fillTimedTables(profile.voltageDeltaInterval, profile.temperatureDeltaInterval);
    calculateMeanVoltage();
    savePotentialMaxVoltage();

    int potentialResult = checkForTerminalValues(profile);
    int result = 0;

    if( potentialResult == 1)
    {
        ++endingStateCounter;
        if(endingStateCounter >= REQUIRED_RESULTS_TO_END)
        {
            result = 1;
        }
    }
    else if( potentialResult == 2)
    {
        result = 1;
    }
    else if( potentialResult == -1)
    {
        ++wrongStateCounter;
        if(wrongStateCounter >= REQUIRED_RESULTS_TO_END)
        {
            result = -1;
        }
    }
    else
    {
        resetStateCounters();
    }

    return result;
}

int ChargingMonitor::checkForTerminalValues(const ChargingProfile& profile ) const
{
    int result = 0;
    if(profile.method == ChargingMethod::constantCurrent)
    {
        if(meanBatteryVoltage >= profile.desiredVoltage)
        {
            result = 1;
            Serial.print("balz1");
        }
    }
    else if(profile.method == ChargingMethod::constantVoltage)
    {
        const double PERMISSIBLE_ERROR = 0.05;
        if(Sensors::batteryVoltage < profile.desiredVoltage - profile.desiredVoltage * PERMISSIBLE_ERROR
        || Sensors::batteryVoltage > profile.desiredVoltage + profile.desiredVoltage * PERMISSIBLE_ERROR)
        {
            result = -1;
            Serial.print("balz2");
        }
        if(Sensors::current <= profile.desiredCurrent)
        {
            result = 1;
            Serial.print("balz3");
        }
    }

    if(finalPhase && profile.lookForEndingVoltageDrop)
    {
        if(maxRecordedVoltage - meanBatteryVoltage >= profile.endingVoltageDrop)
        {
            result = 1;
            Serial.print("balz4");
        }
    }

    if( maxProfileValueHasBeenExceeded(profile) )
    {
        result = -1;
        Serial.print("balz5");
    }

    if(newVoltageData)
    {
        for(int i = 1; i <= REQUIRED_INTERVAL_RESULTS_TO_END; ++i)
        {
            double voltageDelta = voltageInIntervals[voltageTableIter-i] - voltageInIntervals[voltageTableIter-(i-1)];
            if( voltageDelta > profile.minVoltageDelta )
            {
                break;
            }
            if( i == REQUIRED_INTERVAL_RESULTS_TO_END)
            {
                result = 2;
                Serial.print("balz6");
            }
        }
    }
    if(newTemperatureData)
    {
        for(int i = 1; i <= REQUIRED_INTERVAL_RESULTS_TO_END; ++i)
        {
            double temperatureDelta = temperatureInIntervals[temperatureTableIter-i] - temperatureInIntervals[temperatureTableIter-(i-1)];
            if( temperatureDelta < profile.maxTemperatureDelta )
            {
                break;
            }
            if( i == REQUIRED_INTERVAL_RESULTS_TO_END)
            {
                result = 2;
                Serial.print("balz7");
            }
        }
    }

    if(profileChargingTime >= profile.maxTime)
    {
        result = 2;
        Serial.print("balz8");
    }

    return result;
}

void ChargingMonitor::checkTimer()
{
    if(profileStartTime == 0)
        profileStartTime = millis();

    profileChargingTime = getPassedTime(profileStartTime);
}

void ChargingMonitor::fillTimedTables(const unsigned long voltageInvervalTime, const unsigned long temperatureInvervalTime)
{
    unsigned long now = millis();
    if( voltageInvervalTime != 0 && voltageTimer + voltageInvervalTime < now )
    {
        voltageInIntervals[voltageTableIter.at()] = meanBatteryVoltage;
        newVoltageData = true;
        voltageTimer = now;
        ++voltageTableIter;
    }
    else
    {
        newVoltageData = false;
    }

    if( temperatureInvervalTime != 0 && temperatureTimer + temperatureInvervalTime < now )
    {
        temperatureInIntervals[temperatureTableIter.at()] = Sensors::batteryTemperature;
        newTemperatureData = true;
        temperatureTimer = now;
        ++temperatureTableIter;
    }
    else
    {
        newTemperatureData = false;
    }
}

void ChargingMonitor::calculateMeanVoltage()
{
    if( Sensors::current <= 10 )
        return;

    meanVoltageTable[meanTableIter.at()] = Sensors::batteryVoltage;
    ++meanTableIter;
    
    meanBatteryVoltage = 0;
    for(int i = 0; i < MEAN_TABLE_SIZE; ++i)
    {
        meanBatteryVoltage += meanVoltageTable[i];
    }
    meanBatteryVoltage /= MEAN_TABLE_SIZE;
}

void ChargingMonitor::savePotentialMaxVoltage()
{
    if(meanBatteryVoltage > maxRecordedVoltage)
    {
        maxRecordedVoltage = meanBatteryVoltage;
    }
}

void ChargingMonitor::batteryChargingStarted()
{
    unsigned int now = millis();
    batteryStartTime = now;
    profileStartTime = 0;
    voltageTableIter = 0;
    voltageTimer = now;
    newVoltageData = false;
    temperatureTimer = now;
    temperatureTableIter = 0;
    newTemperatureData = false;
    meanTableIter = 0;

    clearBufferTables();
}

void ChargingMonitor::batteryChargingEnded()
{
    batteryChargingTime = getPassedTime(batteryStartTime);
}

void ChargingMonitor::profileChargingEnded()
{
    profileStartTime = 0;
    resetStateCounters();
}

void ChargingMonitor::resetStateCounters()
{
    endingStateCounter = 0;
    wrongStateCounter = 0;
}

unsigned long ChargingMonitor::getPassedTime(const unsigned startTime) const
{
    unsigned long currentTime = millis();
    if(currentTime >= startTime) //timer overflow protection
        return currentTime - startTime;
    else
        return currentTime + UINT32_MAX - startTime;
}

void ChargingMonitor::clearBufferTables()
{
    for(int i = 0; i < TIMED_TABLE_SIZE; ++i)
    {
        voltageInIntervals[i] = 0;
        temperatureInIntervals[i] = 0;
    }
    for(int i = 0; i < MEAN_TABLE_SIZE; ++i)
    {
        meanVoltageTable[i] = 0;
    }
}

bool ChargingMonitor::maxProfileValueHasBeenExceeded(const ChargingProfile& profile) const
{
    return Sensors::current > profile.maxCurrent || 
    meanBatteryVoltage > profile.maxVoltage || 
    Sensors::batteryTemperature > profile.maxTemperature;
}