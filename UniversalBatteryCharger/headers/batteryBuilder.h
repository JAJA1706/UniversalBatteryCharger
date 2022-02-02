#pragma once
#include "charger.h"
#include <Arduino.h>

class BatteryBuilder
{
private:
    Charger* charger;
    ChargingProfile buildedBatteryProfiles[Battery::MAX_NUM_OF_STEPS];
    Battery newBattery;
    BatteryMode newMode;
    volatile bool newBatteryReady;

    int profilesIterator;
    bool buildingProfiles;
    int batteryCanal;
    bool batteryReady;

    static String I2Cbuffer;
    static String I2CString;
public:

    BatteryBuilder();
    void setLinkedCharger(Charger* chargerPtr);
    void putBatteryInLinkedCharger();
    void buildBattery(const String& batteryJSON);
    void buildBatteryProfile(const String& profileJSON);
    bool isNewBatteryReady();
    static void onReceiveString(int bytes);
};

extern BatteryBuilder builder;