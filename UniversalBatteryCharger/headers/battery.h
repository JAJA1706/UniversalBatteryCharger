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
    double capacity;
    BatteryMode mode;
public:
    Battery();
    Battery(const double minVoltage, const double capac, const ChargingProfile profile);
    Battery(const int numOfProfiles, const double minVoltage, const double capac, const ChargingProfile* profiles);
    ChargingProfile& getOngoingChargingProfile();
    bool isCharged() const;
    bool isDischarged() const;
    double getMinVoltage() const;
    BatteryMode getMode() const;
    void setMode(const BatteryMode newMode);
    void moveToNextStep();
    bool isChargingCompleted();

};
