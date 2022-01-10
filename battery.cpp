#include "headers/battery.h"

constexpr int Battery::MAX_NUM_OF_STEPS;

Battery::Battery() : 
    numberOfSteps(0) ,currentStep(0), steps{},
    minimalVoltage(0), capacity(0), mode(BatteryMode::Wait)
{}

Battery::Battery(const double minVoltage, const double capac, const ChargingProfile profile) : 
    numberOfSteps(1), currentStep(0), steps{profile},
    minimalVoltage(minVoltage), capacity(capac), mode(BatteryMode::Wait)
{}

Battery::Battery(const int numOfProfiles, const double minVoltage, const double capac, const ChargingProfile* profiles) : 
    currentStep(0), minimalVoltage(minVoltage), capacity(capac), mode(BatteryMode::Wait)
{
    if(numOfProfiles > MAX_NUM_OF_STEPS)
        numberOfSteps = 4;
    else
        numberOfSteps = numOfProfiles;

    for(int i = 0; i < numberOfSteps; ++i)
    {
        steps[i] = profiles[i];
    }
}

bool Battery::isCharged() const
{
    return mode == BatteryMode::Charge;
}

bool Battery::isDischarged() const
{
    return mode == BatteryMode::Discharge;
}

double Battery::getMinVoltage() const
{
    return minimalVoltage;
}

BatteryMode Battery::getMode() const
{
    return mode;
}

void Battery::setMode(const BatteryMode newMode)
{
    if(mode == BatteryMode::Charge)
        currentStep = 0;

    mode = newMode;
}

ChargingProfile& Battery::getOngoingChargingProfile()
{
    return steps[currentStep];
}

void Battery::moveToNextStep()
{
    ++currentStep;
}

bool Battery::isChargingCompleted()
{
    if(currentStep == numberOfSteps)
        return true;

    return false;
}