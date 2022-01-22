#pragma once
#include "chargingProfile.h"
#include "iterator.h"

class CurrentRegulator
{
private:
    static constexpr int HISTORY_SIZE = 16;
    const int MAX_CIRCUIT_CURRENT;
    const int MAX_POWER_DISSIP_TRANSISTOR;
    const int MAX_POWER_DISSIP_REGULATOR;
    const int PWM_PIN_TRANSISTOR;
	const int PWM_PIN_REGULATOR;
    const int ANALOG_WRITE_MAX;
    const int TRANSISTOR_NEARLY_OPEN_VAL;
    const int REGULATOR_STEPS_EQUAL_TO_TRANSISTOR;
    const double MAX_REGULATOR_ADJUST_VOLTAGE;
    const double REGULATOR_REFERENCE_VOLTAGE;
    int transValue;
    int regulValue;

    InfiniteIterator transValueIter;
    int transValueHistory[HISTORY_SIZE];
    int startingRegulValue;
    unsigned int loopCount;
    bool stabilizeCurrent;

    void adjustRegulator(const int val);
    void adjustTransistor(const int val);
    void adjustComponent(int* component, const int val);
    void adjustOutputsAccordingToInputs( const ChargingProfile& profile );
    void checkLoop();
    void clearTransHistory();
    void balancePowerDissipation();
public:
    CurrentRegulator();
    void batteryChargingStarted(const double desiredVoltage);
    void batteryChargingEnded();
    void applyProfile( const ChargingProfile& profile );
};