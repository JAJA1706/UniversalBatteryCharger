#pragma once
#include "chargingProfile.h"

class CurrentRegulator
{
private:
    const int MAX_CIRCUIT_CURRENT;
    const int MAX_POWER_DISSIP_TRANSISTOR;
    const int MAX_POWER_DISSIP_REGULATOR;
    const int PWM_PIN_TRANSISTOR;
	const int PWM_PIN_REGULATOR;
    const int ANALOG_WRITE_MAX;
    const int TRANSISTOR_NEARLY_OPEN_VAL;
    const double MAX_REGULATOR_ADJUST_VOLTAGE;
    const double REGULATOR_REFERENCE_VOLTAGE;
    int transValue;
    int regulValue;

    unsigned int loopCount;

    void adjustRegulator(const int val);
    void adjustTransistor(const int val);
    void adjustComponent(int* component, const int val);
    void adjustOutputsAccordingToInputs( const ChargingProfile& profile );
    void checkLoop();
    void balancePowerDissipation();
public:
    CurrentRegulator();
    void batteryChargingStarted(const double desiredVoltage);
    void batteryChargingEnded();
    void applyProfile( const ChargingProfile& profile );
};