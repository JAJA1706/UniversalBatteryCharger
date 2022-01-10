#include "headers/currentRegulator.h"
#include "headers/sensors.h"
#include <Arduino.h>

CurrentRegulator::CurrentRegulator() :
    MAX_CIRCUIT_CURRENT(1500),
    MAX_POWER_DISSIP_TRANSISTOR(20000), //mW
    MAX_POWER_DISSIP_REGULATOR(11000), //mW
    PWM_PIN_TRANSISTOR(4),
    PWM_PIN_REGULATOR(13),
    ANALOG_WRITE_MAX(254),
    TRANSISTOR_NEARLY_OPEN_VAL(60),
    MAX_REGULATOR_ADJUST_VOLTAGE(10000),
    REGULATOR_REFERENCE_VOLTAGE(1250),
    transValue(0),
    regulValue(ANALOG_WRITE_MAX),
    loopCount(0),
    stabilizeCurrent(false)
{
    pinMode(PWM_PIN_TRANSISTOR, OUTPUT);
    pinMode(PWM_PIN_REGULATOR, OUTPUT);
}

void CurrentRegulator::applyProfile( const ChargingProfile& profile )
{
    checkLoop();

    const int POWER_CHECK_RATIO = 10;
    const int BALANCE_OUTPUT_RATIO = 2;
    if(loopCount % POWER_CHECK_RATIO == 0)
    {
        balancePowerDissipation();
    }

    adjustOutputsAccordingToInputs(profile);

    analogWrite(PWM_PIN_TRANSISTOR, transValue);
    analogWrite(PWM_PIN_REGULATOR, regulValue);
}

void CurrentRegulator::adjustOutputsAccordingToInputs( const ChargingProfile& profile )
{
    if(Sensors::current > MAX_CIRCUIT_CURRENT)
    {
        adjustTransistor(-2);
    }

    stabilizeCurrent = false;
    if(profile.method == ChargingMethod::constantCurrent || profile.method == ChargingMethod::trickleCharge)
    {
        const double CLOSE_TO_DESIRED_VALUE = 0.1;
        if(Sensors::current > profile.desiredCurrent - profile.desiredCurrent * CLOSE_TO_DESIRED_VALUE
        && Sensors::current < profile.desiredCurrent + profile.desiredCurrent * CLOSE_TO_DESIRED_VALUE)
            stabilizeCurrent = true;
    }
    else if(profile.method == ChargingMethod::constantVoltage)
    {
        const double CLOSE_TO_DESIRED_VALUE = 0.05;
        if(Sensors::batteryVoltage > profile.desiredVoltage - profile.desiredVoltage * CLOSE_TO_DESIRED_VALUE
        && Sensors::batteryVoltage < profile.desiredVoltage + profile.desiredVoltage * CLOSE_TO_DESIRED_VALUE)
            stabilizeCurrent = true;
    }

    const double MIN_REGUL_BATTERY_DIFF = 0.4; 
    if(Sensors::powerResistorVoltageDrop + Sensors::transistorVoltageDrop < MIN_REGUL_BATTERY_DIFF)
    {
        adjustRegulator(1); //is this needed?
    }

    int* component;
    if(stabilizeCurrent || loopCount % 3)
    {
        component = &regulValue;
    }
    else
    {
        component = &transValue;
    }

    if(profile.method == ChargingMethod::constantCurrent || profile.method == ChargingMethod::trickleCharge)
    {
        const double POSSIBLE_ERROR = 0.01;
        if(Sensors::current < profile.desiredCurrent - profile.desiredCurrent * POSSIBLE_ERROR)
            adjustComponent(component, 1);
        if(Sensors::current > profile.desiredCurrent + profile.desiredCurrent * POSSIBLE_ERROR)
            adjustComponent(component, -1);
    }
    else if(profile.method == ChargingMethod::constantVoltage)
    {
        const double POSSIBLE_ERROR = 0.004;
        if(Sensors::batteryVoltage < profile.desiredVoltage - profile.desiredVoltage * POSSIBLE_ERROR)
            adjustComponent(component, 1);
        if(Sensors::batteryVoltage > profile.desiredVoltage + profile.desiredVoltage * POSSIBLE_ERROR)
            adjustComponent(component, -1);
    }
}

void CurrentRegulator::balancePowerDissipation()
{
    double powerDissOnRegulator = Sensors::current * Sensors::regulatorVoltageDrop / 1000;
    if(powerDissOnRegulator > MAX_POWER_DISSIP_REGULATOR)
    {
        adjustRegulator(4);
        adjustTransistor(-4);
    }

    double powerDissOnTransistor = Sensors::current * Sensors::transistorVoltageDrop / 1000;
    if(powerDissOnTransistor > MAX_POWER_DISSIP_TRANSISTOR)
    {
        adjustRegulator(-4);
        adjustTransistor(4);
    }

    double balancedPowerDissOnRegulator = powerDissOnRegulator * MAX_POWER_DISSIP_TRANSISTOR / MAX_POWER_DISSIP_REGULATOR;
    if( balancedPowerDissOnRegulator > powerDissOnTransistor * 1.5 )
    {
        adjustRegulator(1);
        adjustTransistor(-1);
    }
    if( balancedPowerDissOnRegulator * 1.5 < powerDissOnTransistor)
    {
        adjustRegulator(-1);
        adjustTransistor(1);
    }

}

void CurrentRegulator::adjustRegulator(const int val)
{
    adjustComponent(&regulValue, val);
}

void CurrentRegulator::adjustTransistor(const int val)
{
    adjustComponent(&transValue, val);
}

void CurrentRegulator::adjustComponent(int* component, const int val)
{
    if( *component + val < 0 )
        *component = 0;
    else if( *component + val > ANALOG_WRITE_MAX)
        *component = ANALOG_WRITE_MAX;
    else
        *component += val;
}

void CurrentRegulator::batteryChargingStarted( const double desiredVoltage )
{
    const double HIGHEST_REGULATOR_OUTPUT = MAX_REGULATOR_ADJUST_VOLTAGE + REGULATOR_REFERENCE_VOLTAGE;
    double startingRegulatorVoltage = desiredVoltage + (HIGHEST_REGULATOR_OUTPUT - desiredVoltage) / 2;
    regulValue = startingRegulatorVoltage / HIGHEST_REGULATOR_OUTPUT * ANALOG_WRITE_MAX;
    if( regulValue > ANALOG_WRITE_MAX )
        regulValue = ANALOG_WRITE_MAX;
    if (regulValue < 0)
        regulValue = 0;

    transValue = TRANSISTOR_NEARLY_OPEN_VAL;
}

void CurrentRegulator::batteryChargingEnded()
{
    int regulatorValue = (Sensors::batteryVoltage - REGULATOR_REFERENCE_VOLTAGE) / MAX_REGULATOR_ADJUST_VOLTAGE * ANALOG_WRITE_MAX;
    ++regulatorValue;
    if( regulatorValue > ANALOG_WRITE_MAX )
        regulatorValue = ANALOG_WRITE_MAX;
    if (regulatorValue < 0)
        regulatorValue = 0;

    analogWrite(PWM_PIN_TRANSISTOR, 0);
    analogWrite(PWM_PIN_REGULATOR, regulatorValue);
}

void CurrentRegulator::checkLoop()
{
    if(loopCount >= 100)
        loopCount = 0;

    ++loopCount;
}