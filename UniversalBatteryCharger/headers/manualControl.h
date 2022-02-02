#pragma once
#include "battery.h"

class ManualControl
{
private:
    const int LED_PIN[2];
    const int BUTTON_PIN;
    Battery* linkedBattery;
public:
    ManualControl(const int ledpin, const int ledpin2, const int buttonpin);
    void linkBattery(Battery* _newBatteryPtr);
    BatteryMode checkForNewBatteryState();
};
