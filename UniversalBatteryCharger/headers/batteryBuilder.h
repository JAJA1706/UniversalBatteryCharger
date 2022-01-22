#pragma once
#include "charger.h"
#include <Arduino.h>

class BatteryBuilder
{
private:
    Charger* charger;
    ChargingProfile buildedBatteryProfiles[Battery::MAX_NUM_OF_STEPS];
    int profilesIterator;
    bool buildingProfiles;
    int batteryCanal;

    static String I2Cbuffer;
    static String I2CString;
public:
    BatteryBuilder();
    void setLinkedCharger(Charger* chargerPtr);
    void putBatteryInCharger(const Battery& newBattery);
    void buildBattery(const String& batteryJSON);
    void buildBatteryProfile(const String& profileJSON);
    static void onReceiveString(int bytes);
};

extern BatteryBuilder builder;