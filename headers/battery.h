#pragma once
#include "chargingProfile.h"

enum class BatteryMode
{
    Charge,
    Discharge,
    Wait
};

class Battery
{
private:
    static constexpr int MAX_NUM_OF_STEPS = 4;
    int numberOfSteps;
    int currentStep;
    ChargingProfile steps[MAX_NUM_OF_STEPS];

    double minimalVoltage;
    double voltage;
    BatteryMode mode;
public:
    Battery();
    Battery(const int stepsNum, const double minVoltage, const double Voltage, const ChargingProfile profile);
    ChargingProfile& getOngoingChargingProfile();
    bool isCharged() const;
    bool isDischarged() const;
    double getMinVoltage() const;
    double getNominalVoltage() const;
    BatteryMode getMode() const;
    void setMode(const BatteryMode newMode);
    void moveToNextStep();
    bool isChargingCompleted();

};
