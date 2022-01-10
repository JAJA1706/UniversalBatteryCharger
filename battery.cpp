#include "headers/battery.h"

constexpr int Battery::MAX_NUM_OF_STEPS;

Battery::Battery() : 
    numberOfSteps(0) ,currentStep(0), steps{},
    minimalVoltage(0), voltage(0), mode(BatteryMode::Wait)
{}

Battery::Battery(const int stepsNum, const double minVoltage, const double volt, const ChargingProfile profile) : 
    currentStep(0), steps{profile},
    minimalVoltage(minVoltage), voltage(volt), mode(BatteryMode::Wait)
{
    if(stepsNum > MAX_NUM_OF_STEPS)
        numberOfSteps = 4;
    else
        numberOfSteps = stepsNum;
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

double Battery::getNominalVoltage() const
{
    return voltage;
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